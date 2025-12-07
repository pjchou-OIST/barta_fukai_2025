import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from sklearn.manifold import MDS
from sklearn.metrics import pairwise_distances
from math import pi
import os
import sys
from tqdm import tqdm

# 設定風格
sns.set_context("talk")
plt.rcParams.update({'font.size': 14})

# --- 1. 基礎運算函數 ---
def load_patterns_from_file(filename):
    if not os.path.exists(filename):
        print(f"❌ Error: Pattern file not found: {filename}")
        return None, None
    with h5py.File(filename, "r") as h5f:
        if 'connectivity/patterns/indices' not in h5f: return None, None
        indices = h5f['connectivity/patterns/indices'][:]
        splits = h5f['connectivity/patterns/splits'][:]
        N_exc = h5f['connectivity'].attrs.get('N_exc', 8000)
    patterns = [indices[splits[i]:splits[i+1]] for i in range(len(splits)-1)]
    return patterns, int(N_exc)

def get_group_similarity_order(pattern_file, npat, group_size):
    """
    針對特定的 init 檔，計算 Group 相似度與排序
    """
    patterns, N_neurons = load_patterns_from_file(pattern_file)
    if patterns is None: return None, None
    if len(patterns) < npat: npat = len(patterns)
    
    n_groups = npat // group_size
    group_vectors = np.zeros((n_groups, N_neurons), dtype=bool)
    for g in range(n_groups):
        for p_idx in range(g*group_size, (g+1)*group_size):
            group_vectors[g, patterns[p_idx]] = True
            
    # MDS 1D Embedding
    # 這裡不設 random_state，或者設為固定值都可以，重點是反映該次模擬的真實結構
    dist_matrix = pairwise_distances(group_vectors, metric='jaccard')
    mds = MDS(n_components=1, dissimilarity='precomputed', random_state=42, normalized_stress=False)
    x_coords = mds.fit_transform(dist_matrix).flatten()
    
    # 正規化 0-100
    x_coords = (x_coords - x_coords.min()) / (x_coords.max() - x_coords.min()) * 100
    
    # 取得排序索引 (相似的 Group 排在一起)
    sorted_indices = np.argsort(x_coords)
    return sorted_indices, x_coords

def analyze_model(label, act_path, npat, group_size, start_time):
    # --- 動態尋找對應的 init 檔案 ---
    base_dir = os.path.dirname(act_path)
    init_path = os.path.join(base_dir, f"init{npat}.h5")
    
    print(f"Processing: {label}")
    # print(f"  - Init: {os.path.basename(init_path)}")
    
    # 1. 針對這個模型，計算它自己的排序與座標
    sorted_groups, x_coords = get_group_similarity_order(init_path, npat, group_size)
    if sorted_groups is None:
        print(f"⚠️ Skipping {label}: init file not found.")
        return None, None

    if not os.path.exists(act_path): return None, None

    with h5py.File(act_path, "r") as h5f:
        if 'activations' not in h5f: return None, None
        events = h5f['activations'][:] 
    
    # Sort & Filter
    raw_times = events[0] * 0.01
    raw_ids = events[2].astype(int)
    sort_idx = np.argsort(raw_times)
    
    times = raw_times[sort_idx]
    ids = raw_ids[sort_idx]
    
    mask = times >= start_time
    ids = ids[mask]
    
    if len(ids) < 2: return None, None

    groups = ids // group_size
    phases = ids % group_size
    
    # --- 計算 Radar Metrics ---
    counts = {'Fwd': 0, 'Rev': 0, 'Jump': 0}
    jump_dists = []
    
    for i in range(len(ids) - 1):
        if groups[i] == groups[i+1]:
            if phases[i+1] == (phases[i] + 1) % group_size: counts['Fwd'] += 1
            elif phases[i+1] == (phases[i] - 1) % group_size: counts['Rev'] += 1
            else: counts['Jump'] += 1
        else:
            counts['Jump'] += 1
            # 使用該模型自己的 x_coords 計算距離
            g1, g2 = groups[i], groups[i+1]
            if g1 < len(x_coords) and g2 < len(x_coords):
                d = abs(x_coords[g1] - x_coords[g2])
                max_d = 100 # 因為我們正規化過
                jump_dists.append(d / max_d)

    total = sum(counts.values())
    avg_jump_dist = np.mean(jump_dists) if jump_dists else 1.0
    
    radar_data = {
        'Sequence Fidelity': counts['Fwd'] / total,
        'Reverse Replay': counts['Rev'] / total,
        'Associativity': counts['Jump'] / total,
        'Jump Precision': 1.0 - avg_jump_dist, 
    }

    # --- 計算 Group Transition Matrix (使用自己的 sorted_groups) ---
    n_groups = len(sorted_groups)
    # 建立映射: 原始 Group ID -> 排序後的新 ID (0~99)
    group_map = {original_id: new_idx for new_idx, original_id in enumerate(sorted_groups)}
    
    trans_mat = np.zeros((n_groups, n_groups))
    
    for i in range(len(ids) - 1):
        g_curr = groups[i]
        g_next = groups[i+1]
        
        if g_curr in group_map and g_next in group_map:
            idx_curr = group_map[g_curr]
            idx_next = group_map[g_next]
            trans_mat[idx_next, idx_curr] += 1 # Y=Next, X=Curr
            
    # Log scale for visibility
    trans_mat = np.log1p(trans_mat) 
    
    return radar_data, trans_mat

# --- 2. 畫雷達圖 ---
def plot_radar(data_dict, output_dir):
    categories = list(data_dict[list(data_dict.keys())[0]].keys())
    N = len(categories)
    angles = [n / float(N) * 2 * pi for n in range(N)]
    angles += angles[:1]
    
    fig, ax = plt.subplots(figsize=(8, 8), subplot_kw={'projection': 'polar'})
    colors = ['gray', 'crimson', 'royalblue', 'darkorange', 'green', 'purple']
    
    for i, (label, metrics) in enumerate(data_dict.items()):
        values = [metrics[cat] for cat in categories]
        values += values[:1]
        ax.plot(angles, values, linewidth=2, linestyle='solid', label=label, color=colors[i])
        ax.fill(angles, values, color=colors[i], alpha=0.25)
    
    plt.xticks(angles[:-1], categories, size=12)
    ax.set_rlabel_position(0)
    plt.yticks([0.2, 0.4, 0.6, 0.8], ["0.2", "0.4", "0.6", "0.8"], color="grey", size=10)
    plt.ylim(0, 1)
    plt.title("Model Personality Profile", size=16, fontweight='bold', y=1.1)
    plt.legend(loc='upper right', bbox_to_anchor=(1.3, 1.1))
    
    save_path = os.path.join(output_dir, "Poster_Radar.png")
    plt.savefig(save_path, bbox_inches='tight', dpi=300)
    print(f"✅ Radar chart saved: {save_path}")

# --- 3. 畫 Group 熱圖 ---
def plot_group_matrices(matrices, output_dir):
    n_models = len(matrices)
    fig, axes = plt.subplots(1, n_models, figsize=(7*n_models, 6))
    
    for i, (label, mat) in enumerate(matrices.items()):
        ax = axes[i] if n_models > 1 else axes
        sns.heatmap(mat, ax=ax, cmap="magma", square=True, cbar=True, xticklabels=False, yticklabels=False)
        ax.set_title(f"{label}\nGroup Transitions\n(Self-sorted by Similarity)", fontweight='bold')
        ax.set_xlabel("From Group (Sorted)")
        ax.set_ylabel("To Group (Sorted)")
        ax.invert_yaxis()
        
    plt.tight_layout()
    save_path = os.path.join(output_dir, "Poster_Group_Matrix.png")
    plt.savefig(save_path, bbox_inches='tight', dpi=300)
    print(f"✅ Group Matrix saved: {save_path}")

# --- Main ---
if __name__ == '__main__':
    # Config
    npat = 1000
    group_size = 10
    start_time = 50.0 # Skip warmup
    
    # 這裡填入絕對路徑，程式會自動去該目錄找 init1000.h5
    model_files = {
        "base": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/base_hebb_spontaneous1000_activations.h5",
        "SFA-only": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/sfa_hebb_spontaneous1000_activations.h5",
        "dist-STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/std_hebb_spontaneous_short1000_activations.h5",
        "SFA+STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/mix_hebb_spontaneous_short1000_activations.h5"
    }
    
    output_dir = "poster_plots/advanced/"
    if not os.path.exists(output_dir): os.makedirs(output_dir)
    
    # 2. 分析模型 (獨立計算)
    radar_data = {}
    matrices = {}
    
    for label, path in tqdm(model_files.items()):
        res, mat = analyze_model(label, path, npat, group_size, start_time)
        if res:
            radar_data[label] = res
            matrices[label] = mat
            
    # 3. 繪圖
    if radar_data:
        plot_radar(radar_data, output_dir)
        plot_group_matrices(matrices, output_dir)
    else:
        print("❌ No data processed.")