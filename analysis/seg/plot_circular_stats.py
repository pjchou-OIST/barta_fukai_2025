import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from sklearn.manifold import MDS
from sklearn.metrics import pairwise_distances
import os
import sys
import argparse
from tqdm import tqdm

# 設定風格
sns.set_context("talk")
plt.rcParams.update({'font.size': 14})

# --- 1. 載入 Pattern 與 計算 MDS ---
def load_patterns_from_file(filename):
    if not os.path.exists(filename):
        print(f"❌ Pattern file not found: {filename}")
        return None, None
    with h5py.File(filename, "r") as h5f:
        if 'connectivity/patterns/indices' not in h5f: return None, None
        indices = h5f['connectivity/patterns/indices'][:]
        splits = h5f['connectivity/patterns/splits'][:]
        if 'connectivity' in h5f and 'N_exc' in h5f['connectivity'].attrs:
            N_exc = h5f['connectivity'].attrs['N_exc']
        else:
            N_exc = 8000
    patterns = []
    for i in range(len(splits) - 1):
        patterns.append(indices[splits[i]:splits[i+1]])
    return patterns, int(N_exc)

def calculate_group_similarity_map(pattern_file_path, npat, group_size=10):
    patterns, N_neurons = load_patterns_from_file(pattern_file_path)
    if patterns is None: return None
    if len(patterns) < npat: npat = len(patterns)
    
    n_groups = npat // group_size
    group_vectors = np.zeros((n_groups, N_neurons), dtype=bool)
    for g in range(n_groups):
        for p_idx in range(g*group_size, (g+1)*group_size):
            group_vectors[g, patterns[p_idx]] = True
            
    dist_matrix = pairwise_distances(group_vectors, metric='jaccard')
    mds = MDS(n_components=1, dissimilarity='precomputed', random_state=42, normalized_stress=False)
    x_coords = mds.fit_transform(dist_matrix).flatten()
    # Normalize 0-100
    return (x_coords - x_coords.min()) / (x_coords.max() - x_coords.min()) * 100

# --- 2. 計算統計數據 ---
def calculate_stats(label, act_path, x_map, npat, group_size, start_time, max_gap=0.2):
    if not os.path.exists(act_path):
        print(f"❌ File not found: {act_path}")
        return None

    with h5py.File(act_path, "r") as h5f:
        if 'activations' not in h5f: return None
        events = h5f['activations'][:] 
        
    raw_times = events[0]
    raw_durs = events[1]
    raw_ids = events[2].astype(int)
    
    # Sort
    sort_idx = np.argsort(raw_times)
    sorted_ids = raw_ids[sort_idx]
    sorted_start = raw_times[sort_idx] * 0.01
    sorted_end = sorted_start + (raw_durs[sort_idx] * 0.01)
    
    # Filter Time
    mask = sorted_start >= start_time
    ids = sorted_ids[mask]
    starts = sorted_start[mask]
    ends = sorted_end[mask]
    
    if len(ids) < 2: return None

    # Stats Containers
    counts = {'Forward': 0, 'Reverse': 0, 'Jump': 0}
    jump_distances = []
    
    groups = ids // group_size
    phases = ids % group_size
    
    for i in range(len(ids) - 1):
        gap = starts[i+1] - ends[i]
        if gap > max_gap: continue # Skip if sequence broken

        curr_g, curr_p = groups[i], phases[i]
        next_g, next_p = groups[i+1], phases[i+1]
        
        # Classification
        if curr_g == next_g:
            if next_p == (curr_p + 1) % group_size:
                counts['Forward'] += 1
            elif next_p == (curr_p - 1) % group_size:
                counts['Reverse'] += 1
            else:
                counts['Jump'] += 1 
        else:
            counts['Jump'] += 1
            # 計算跨組跳躍距離 (Cross-talk distance)
            if curr_g < len(x_map) and next_g < len(x_map):
                dist = abs(x_map[curr_g] - x_map[next_g])
                jump_distances.append(dist)

    total = sum(counts.values())
    if total == 0: return None
    
    # Return normalized ratios
    return {
        'Model': label,
        'Forward': counts['Forward'] / total,
        'Reverse': counts['Reverse'] / total,
        'Jump': counts['Jump'] / total,
        'Jump Distances': jump_distances,
        'Total Transitions': total
    }

# --- 3. 繪圖主函數 ---
def plot_circular_stats(model_configs, output_dir, npat, group_size, start_time):
    print("Calculating stats...")
    
    stats_list = []
    all_jump_dists = [] 
    
    # 迴圈處理每個模型
    for label, act_path in tqdm(model_configs.items()):
        
        # --- A. 動態尋找 init 檔案 ---
        base_dir = os.path.dirname(act_path)
        init_path = os.path.join(base_dir, f"init{npat}.h5")
        
        # --- B. 計算該模型的 X 軸 (MDS) ---
        x_map = calculate_group_similarity_map(init_path, npat, group_size)
        
        if x_map is None:
            print(f"⚠️ Skipping {label}: init file not found or invalid.")
            continue
            
        # --- C. 計算統計數據 ---
        res = calculate_stats(label, act_path, x_map, npat, group_size, start_time)
        
        if res:
            stats_list.append(res)
            # Flatten jump distances for dataframe
            for d in res['Jump Distances']:
                all_jump_dists.append({'Model': label, 'Distance': d})
    
    if not stats_list:
        print("❌ No valid data generated.")
        return

    df_props = pd.DataFrame(stats_list)
    df_jumps = pd.DataFrame(all_jump_dists)
    
    # --- Plotting ---
    fig, axes = plt.subplots(1, 2, figsize=(16, 7))
    
    # Plot 1: Stacked Bar (Transition Types)
    ax = axes[0]
    df_melt = df_props.melt(id_vars="Model", value_vars=['Forward', 'Reverse', 'Jump'], 
                            var_name="Type", value_name="Proportion")
    
    colors = {'Forward': 'crimson', 'Reverse': 'forestgreen', 'Jump': 'royalblue'}
    
    # 使用 Pandas plot 畫 Stacked Bar
    df_props.set_index('Model')[['Forward', 'Reverse', 'Jump']].plot(
        kind='bar', stacked=True, ax=ax, color=[colors['Forward'], colors['Reverse'], colors['Jump']], width=0.6
    )
    
    ax.set_title("A. Transition Profile", fontweight='bold')
    ax.set_ylabel("Proportion")
    ax.set_xlabel("")
    ax.set_ylim(0, 1.0)
    ax.legend(title="Type", loc='upper right', bbox_to_anchor=(1.3, 1))
    
    # Add text labels
    for c in ax.containers:
        # 只顯示 > 0.05 的數值，避免太擠
        labels = [f'{v.get_height():.2f}' if v.get_height() > 0.05 else '' for v in c]
        ax.bar_label(c, labels=labels, label_type='center', color='white', fontsize=12, fontweight='bold')

    # Plot 2: Jump Distance (Cross-talk Analysis)
    ax = axes[1]
    if not df_jumps.empty:
        sns.violinplot(data=df_jumps, x='Model', y='Distance', ax=ax, palette="pastel", inner="quartile")
        sns.stripplot(data=df_jumps, x='Model', y='Distance', ax=ax, color='black', alpha=0.3, size=3, jitter=True)
        
        ax.set_title("B. Associative Jump Distance\n(Cross-talk Range)", fontweight='bold')
        ax.set_ylabel("Distance in Similarity Space (0-100)")
        ax.set_xlabel("")
        ax.set_ylim(0, 100)
    else:
        ax.text(0.5, 0.5, "No Jumps Detected", ha='center')

    plt.tight_layout()
    save_path = os.path.join(output_dir, "Poster_Circular_Stats_Dynamic.png")
    plt.savefig(save_path, dpi=300)
    print(f"✅ Saved stats plot to: {save_path}")

if __name__ == '__main__':
    # ================= Config =================
    output_dir = "poster_plots/"
    if not os.path.exists(output_dir): os.makedirs(output_dir)
    
    # 請填入 activations.h5 的絕對路徑
    # 程式會自動去找同一目錄下的 init1000.h5
    model_files = {
        "base": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/base_hebb_spontaneous1000_activations.h5",
        "SFA-only": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/sfa_hebb_spontaneous1000_activations.h5",
        "dist-STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/std_hebb_spontaneous_short1000_activations.h5",
        "SFA+STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/mix_hebb_spontaneous_short1000_activations.h5"
    }
    
    # Params
    npat = 1000
    group_size = 10
    start_time = 50.0 # Skip warmup
    
    plot_circular_stats(model_files, output_dir, npat, group_size, start_time)