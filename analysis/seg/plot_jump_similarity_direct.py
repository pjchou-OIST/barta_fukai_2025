import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import pairwise_distances
import os
import sys
from tqdm import tqdm

# 設定風格
sns.set_context("talk")
plt.rcParams.update({'font.size': 14})

# --- 1. 計算真實相似度矩陣 ---
def get_similarity_matrix(pattern_file, npat, group_size=10):
    if not os.path.exists(pattern_file): return None
    
    with h5py.File(pattern_file, "r") as h5f:
        indices = h5f['connectivity/patterns/indices'][:]
        splits = h5f['connectivity/patterns/splits'][:]
        N_exc = h5f['connectivity'].attrs.get('N_exc', 8000)
    
    patterns = [indices[splits[i]:splits[i+1]] for i in range(len(splits)-1)]
    if len(patterns) < npat: npat = len(patterns)

    n_groups = npat // group_size
    group_vectors = np.zeros((n_groups, N_neurons := int(N_exc)), dtype=bool)
    
    for g in range(n_groups):
        for p_idx in range(g*group_size, (g+1)*group_size):
            group_vectors[g, patterns[p_idx]] = True
            
    dist_matrix = pairwise_distances(group_vectors, metric='jaccard')
    sim_matrix = 1.0 - dist_matrix
    return sim_matrix

# --- 2. 收集跳躍數據 ---
def collect_jumps(act_path, group_size=10, start_time=50.0, max_gap=0.2):
    if not os.path.exists(act_path): return []

    with h5py.File(act_path, "r") as h5f:
        if 'activations' not in h5f: return []
        events = h5f['activations'][:] 
        
    raw_times = events[0]
    raw_durs = events[1]
    raw_ids = events[2].astype(int)
    
    sort_idx = np.argsort(raw_times)
    sorted_ids = raw_ids[sort_idx]
    sorted_start = raw_times[sort_idx] * 0.01
    sorted_end = sorted_start + (raw_durs[sort_idx] * 0.01)
    
    mask = sorted_start >= start_time
    ids = sorted_ids[mask]
    starts = sorted_start[mask]
    ends = sorted_end[mask]
    
    groups = ids // group_size
    jump_pairs = []
    
    for i in range(len(ids) - 1):
        gap = starts[i+1] - ends[i]
        if gap > max_gap: continue

        g_curr = groups[i]
        g_next = groups[i+1]
        
        if g_curr != g_next:
            jump_pairs.append((g_curr, g_next))
            
    return jump_pairs

# --- 3. 主分析函數 ---
def plot_direct_similarity(model_configs, output_dir, npat, group_size, start_time):
    print("Calculating Jaccard Similarities...")
    similarity_data = []
    
    # 取得第一個模型的 init 作為 Baseline 參考
    first_path = list(model_configs.values())[0]
    base_dir = os.path.dirname(first_path)
    init_path = os.path.join(base_dir, f"init{npat}.h5")
    sim_matrix_ref = get_similarity_matrix(init_path, npat, group_size)
    
    if sim_matrix_ref is None: return

    # A. Random Baseline Data
    upper_tri = sim_matrix_ref[np.triu_indices_from(sim_matrix_ref, k=1)]
    # 為了讓 KDE 畫得出來，我們隨機抽樣 10000 點就好，不用全部
    baseline_sample = np.random.choice(upper_tri, min(10000, len(upper_tri)), replace=False)
    for val in baseline_sample:
        similarity_data.append({'Type': 'Random Chance', 'Similarity': val})
        
    # B. Model Data
    for label, act_path in tqdm(model_configs.items()):
        my_base_dir = os.path.dirname(act_path)
        my_init_path = os.path.join(my_base_dir, f"init{npat}.h5")
        my_sim_matrix = get_similarity_matrix(my_init_path, npat, group_size)
        
        if my_sim_matrix is None: continue
        
        jumps = collect_jumps(act_path, group_size, start_time)
        for g1, g2 in jumps:
            if g1 < len(my_sim_matrix) and g2 < len(my_sim_matrix):
                sim_val = my_sim_matrix[g1, g2]
                similarity_data.append({'Type': label, 'Similarity': sim_val})

    import pandas as pd
    df = pd.DataFrame(similarity_data)
    
    # --- 繪圖 (只畫一張 KDE) ---
    plt.figure(figsize=(10, 7))
    
    # 定義顏色
    palette = {'Random Chance': 'gray', 'SFA-only': 'orange', 'dist-STD': 'crimson'}
    
    # KDE Plot
    # common_norm=False: 讓每條線都自己歸一化 (Area=1)，方便比較形狀
    # fill=True: 填色比較好看
    sns.kdeplot(data=df, x='Similarity', hue='Type', palette=palette, 
                common_norm=False, fill=True, alpha=0.3, linewidth=3)
    
    # 畫平均線
    means = df.groupby('Type')['Similarity'].mean()
    for name, val in means.items():
        if name in palette:
            plt.axvline(val, color=palette[name], linestyle='--', linewidth=2, label=f"{name} Mean")

    plt.title("Jump Target Similarity Distribution\n(Right Shift = Associative Jump)", fontweight='bold')
    plt.xlabel("Structural Similarity (Jaccard Index)")
    plt.ylabel("Density")
    plt.xlim(0, df['Similarity'].max() * 1.2) # 自動調整範圍
    plt.legend(title="Model / Baseline")
    
    save_path = os.path.join(output_dir, "Poster_Direct_Similarity_KDE.png")
    plt.savefig(save_path, dpi=300)
    print(f"✅ Saved: {save_path}")

if __name__ == '__main__':
    # Config
    npat = 1000
    group_size = 10
    start_time = 50.0 
    output_dir = "poster_plots/"
    if not os.path.exists(output_dir): os.makedirs(output_dir)

    model_files = {
        "SFA-only": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/sfa_hebb_spontaneous1000_activations.h5",
        "dist-STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/std_hebb_spontaneous_short1000_activations.h5"
    }
    
    plot_direct_similarity(model_files, output_dir, npat, group_size, start_time)