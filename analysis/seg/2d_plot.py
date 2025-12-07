import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.manifold import MDS
from sklearn.metrics import pairwise_distances
import os
import sys
import argparse
from tqdm import tqdm

# 設定風格
sns.set_context("talk")
plt.rcParams.update({'font.size': 14})

# --- 1. 自定義 Load Pattern 函數 ---
def load_patterns_from_file(filename):
    if not os.path.exists(filename):
        print(f"❌ Error: Pattern file not found: {filename}")
        return None, None

    with h5py.File(filename, "r") as h5f:
        if 'connectivity/patterns/indices' not in h5f:
            return None, None
            
        indices = h5f['connectivity/patterns/indices'][:]
        splits = h5f['connectivity/patterns/splits'][:]
        
        if 'connectivity' in h5f and 'N_exc' in h5f['connectivity'].attrs:
            N_exc = h5f['connectivity'].attrs['N_exc']
        else:
            N_exc = 8000
            
    patterns = []
    for i in range(len(splits) - 1):
        start = splits[i]
        end = splits[i+1]
        patterns.append(indices[start:end])
        
    return patterns, int(N_exc)

# --- 2. 計算相似度座標 (X軸) ---
def calculate_group_similarity_map(pattern_file_path, npat, group_size=10):
    patterns, N_neurons = load_patterns_from_file(pattern_file_path)
    
    if patterns is None: return None
    if len(patterns) < npat: npat = len(patterns)
    
    n_groups = npat // group_size
    group_vectors = np.zeros((n_groups, N_neurons), dtype=bool)
    
    for g in range(n_groups):
        for p_idx in range(g*group_size, (g+1)*group_size):
            neuron_indices = patterns[p_idx]
            group_vectors[g, neuron_indices] = True
        
    dist_matrix = pairwise_distances(group_vectors, metric='jaccard')
    mds = MDS(n_components=1, dissimilarity='precomputed', random_state=42, normalized_stress=False)
    x_coords = mds.fit_transform(dist_matrix).flatten()
    
    # Normalize 0-100
    x_coords = (x_coords - x_coords.min()) / (x_coords.max() - x_coords.min()) * 100
    return x_coords

# --- 3. 繪圖主函數 ---
def plot_2d_trajectory(label, act_path, output_dir, npat, group_size, start_time, n_plot_events):
    
    # --- 動態尋找 init 檔案 ---
    base_dir = os.path.dirname(act_path)
    init_path = os.path.join(base_dir, f"init{npat}.h5")
    
    # 每個模型用自己的 init 檔重算 X 軸 (確保對應正確)
    x_map = calculate_group_similarity_map(init_path, npat, group_size)
    if x_map is None:
        print(f"❌ Skipping {label} due to missing init file.")
        return

    if not os.path.exists(act_path):
        print(f"❌ File not found: {act_path}")
        return

    with h5py.File(act_path, "r") as h5f:
        if 'activations' not in h5f: return
        events = h5f['activations'][:] 
        
    raw_times = events[0]
    raw_durs = events[1]
    raw_ids = events[2].astype(int)
    
    sort_idx = np.argsort(raw_times)
    sorted_ids = raw_ids[sort_idx]
    
    dt = 0.01
    sorted_start_times = raw_times[sort_idx] * dt
    sorted_durations = raw_durs[sort_idx] * dt
    sorted_end_times = sorted_start_times + sorted_durations

    # Time Filter
    mask = sorted_start_times >= start_time
    ids = sorted_ids[mask][:n_plot_events]
    starts = sorted_start_times[mask][:n_plot_events]
    ends = sorted_end_times[mask][:n_plot_events]
    
    if len(ids) == 0:
        print(f"⚠️ No events found after {start_time}s.")
        return

    # 轉換座標
    groups = ids // group_size
    phases = ids % group_size
    
    valid_mask = groups < len(x_map)
    groups = groups[valid_mask]
    phases = phases[valid_mask]
    ids = ids[valid_mask]
    starts = starts[valid_mask]
    ends = ends[valid_mask]
    
    x = x_map[groups] 
    y = phases        
    
    # Jitter
    x_jitter = x + np.random.normal(0, 0.5, len(x))

    # --- 繪圖 ---
    fig, ax = plt.subplots(figsize=(14, 9))
    
    # 背景網格
    for x_pos in x_map:
        ax.scatter(np.full(group_size, x_pos), np.arange(group_size), 
                   c='lightgray', s=15, alpha=0.3, zorder=0)

    # 畫軌跡
    max_gap = 0.2
    
    # 統計計數器
    stats_count = {'Fwd': 0, 'Rev': 0, 'Jump': 0}
    
    for i in range(len(ids) - 1):
        curr_end = ends[i]
        next_start = starts[i+1]
        gap = next_start - curr_end
        
        if gap > max_gap: continue

        curr_g, curr_p = groups[i], phases[i]
        next_g, next_p = groups[i+1], phases[i+1]

        x1, y1 = x_jitter[i], y[i]
        x2, y2 = x_jitter[i+1], y[i+1]
        
        # --- 顏色判定邏輯 ---
        # 1. Forward (Red): 同組 且 +1
        if curr_g == next_g and next_p == (curr_p + 1) % group_size:
            color = 'crimson'
            alpha = 0.8
            width = 2.5
            zorder = 5
            stats_count['Fwd'] += 1
            
        # 2. Reverse (Green): 同組 且 -1 (注意負數取模)
        elif curr_g == next_g and next_p == (curr_p - 1) % group_size:
            color = 'forestgreen' # 深綠色比較明顯
            alpha = 0.8
            width = 2.5
            zorder = 5
            stats_count['Rev'] += 1
            
        # 3. Jump (Blue): 其他所有情況
        else:
            dist = abs(x2 - x1)
            alpha = max(0.2, 1 - dist/50)
            color = 'royalblue'
            width = 1.5
            zorder = 3
            stats_count['Jump'] += 1
            
        ax.annotate("", xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="-|>", color=color, alpha=alpha, lw=width),
                    zorder=zorder)

    # 畫點
    sc = ax.scatter(x_jitter, y, c=starts, cmap='viridis', s=80, zorder=10, edgecolors='w', alpha=0.9)
    cbar = plt.colorbar(sc)
    cbar.set_label('Time (s)')
    
    actual_duration = starts[-1] - starts[0] if len(starts) > 0 else 0
    ax.set_title(f"Trajectory: {label}\n({len(ids)} events over {actual_duration:.1f}s)", fontweight='bold')
    ax.set_xlabel("Group Similarity Space (MDS Projection)")
    ax.set_ylabel("Circular Phase (0 -> 9)")
    ax.set_yticks(np.arange(10))
    ax.set_ylim(-0.5, 9.5)
    
    # 標註統計數據 (包含綠色的 Reverse)
    stats_text = f"Forward (Red): {stats_count['Fwd']}\nReverse (Green): {stats_count['Rev']}\nJumps (Blue): {stats_count['Jump']}"
    
    ax.text(0.02, 0.95, stats_text, transform=ax.transAxes, 
            bbox=dict(facecolor='white', alpha=0.9, edgecolor='gray'), fontsize=12, verticalalignment='top')

    plt.tight_layout()
    
    safe_label = label.replace(" ", "_").replace("(", "").replace(")", "").replace("+", "_")
    save_path = os.path.join(output_dir, f"Trajectory_{safe_label}_colorV2.png")
    
    plt.savefig(save_path, dpi=150)
    print(f"✅ Saved: {save_path}\n")
    plt.close(fig)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--start', type=float, default=50.0, help="Skip first N seconds")
    parser.add_argument('--n_plot', type=int, default=200, help="Number of events to plot")
    args = parser.parse_args()

    npat = 1000
    group_size = 10
    output_dir = "poster_plots/"
    if not os.path.exists(output_dir): os.makedirs(output_dir)

    # 填入您的檔案路徑
    model_files = {
        "base": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/base_hebb_spontaneous1000_activations.h5",
        "SFA-only": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/sfa_hebb_spontaneous1000_activations.h5",
        "dist-STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/std_hebb_spontaneous_short1000_activations.h5",
        "SFA+STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/mix_hebb_spontaneous_short1000_activations.h5"
    }
    
    for label, path in tqdm(model_files.items()):
        plot_2d_trajectory(label, path, output_dir, npat, group_size, args.start, args.n_plot)
        
    print("Done.")
