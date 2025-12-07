import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from sklearn.manifold import MDS
from sklearn.metrics import pairwise_distances
from matplotlib.gridspec import GridSpec
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib.lines import Line2D
from matplotlib.patches import Patch
import os
import sys
from tqdm import tqdm
from math import pi

# 設定繪圖風格
sns.set_context("talk")
plt.rcParams.update({
    'font.size': 14, 
    'axes.titlesize': 16, 
    'axes.labelsize': 14,
    'xtick.labelsize': 12,
    'ytick.labelsize': 12
})

# ==========================================
# 1. 核心運算函數 (保持不變)
# ==========================================

def load_patterns_from_file(filename):
    if not os.path.exists(filename): return None, None
    with h5py.File(filename, "r") as h5f:
        if 'connectivity/patterns/indices' not in h5f: return None, None
        indices = h5f['connectivity/patterns/indices'][:]
        splits = h5f['connectivity/patterns/splits'][:]
        N_exc = h5f['connectivity'].attrs.get('N_exc', 8000)
    patterns = [indices[splits[i]:splits[i+1]] for i in range(len(splits)-1)]
    return patterns, int(N_exc)

def calculate_mds_x_axis(init_path, npat, group_size):
    patterns, N_neurons = load_patterns_from_file(init_path)
    if patterns is None: return None
    if len(patterns) < npat: npat = len(patterns)
    
    n_groups = npat // group_size
    group_vectors = np.zeros((n_groups, N_neurons), dtype=bool)
    for g in range(n_groups):
        for p_idx in range(g*group_size, (g+1)*group_size):
            if p_idx < len(patterns):
                group_vectors[g, patterns[p_idx]] = True
            
    dist_matrix = pairwise_distances(group_vectors, metric='jaccard')
    mds = MDS(n_components=1, dissimilarity='precomputed', random_state=42, normalized_stress=False)
    x_coords = mds.fit_transform(dist_matrix).flatten()
    return (x_coords - x_coords.min()) / (x_coords.max() - x_coords.min()) * 100

def get_model_data(act_path, npat, group_size, start_time, n_plot=None):
    base_dir = os.path.dirname(act_path)
    init_path = os.path.join(base_dir, f"init{npat}.h5")
    
    x_map = calculate_mds_x_axis(init_path, npat, group_size)
    if x_map is None: return None

    if not os.path.exists(act_path): return None
    with h5py.File(act_path, "r") as h5f:
        if 'activations' not in h5f: return None
        events = h5f['activations'][:] 
    
    raw_times = events[0] * 0.01
    raw_durs = events[1] * 0.01
    raw_ids = events[2].astype(int)
    
    sort_idx = np.argsort(raw_times)
    times = raw_times[sort_idx]
    ids = raw_ids[sort_idx]
    durs = raw_durs[sort_idx]
    
    mask_time = times >= start_time
    times = times[mask_time]
    ids = ids[mask_time]
    durs = durs[mask_time]
    
    # Safety Filter
    groups = ids // group_size
    valid_mask = groups < len(x_map)
    times = times[valid_mask]
    ids = ids[valid_mask]
    durs = durs[valid_mask]
    groups = groups[valid_mask] 
    phases = ids % group_size
    
    if len(ids) < 2: return None

    # A. Plot Data (Subset)
    limit = n_plot if n_plot else len(ids)
    plot_data = {
        'times': times[:limit],
        'ids': ids[:limit],
        'durs': durs[:limit],
        'ends': (times + durs)[:limit],
        'x_map': x_map
    }
    
    # B. Stats Data (Full)
    trans_mat_phase = np.zeros((group_size, group_size))
    sorted_g_idx = np.argsort(x_map)
    g_map = {old: new for new, old in enumerate(sorted_g_idx)}
    trans_mat_group = np.zeros((len(x_map), len(x_map)))
    
    stats = {'Fwd': 0, 'Rev': 0, 'Jump': 0}
    jump_dists = []
    max_gap = 0.2
    
    for i in range(len(ids) - 1):
        gap = times[i+1] - (times[i] + durs[i])
        if gap > max_gap: continue 

        g_curr, p_curr = groups[i], phases[i]
        g_next, p_next = groups[i+1], phases[i+1]
        
        if g_curr == g_next:
            trans_mat_phase[p_next, p_curr] += 1
            if p_next == (p_curr + 1) % group_size: stats['Fwd'] += 1
            elif p_next == (p_curr - 1) % group_size: stats['Rev'] += 1
            else: stats['Jump'] += 1
        else:
            stats['Jump'] += 1
            if g_curr < len(x_map) and g_next < len(x_map):
                d = abs(x_map[g_curr] - x_map[g_next])
                jump_dists.append(d / 100.0) 
        
        if g_curr in g_map and g_next in g_map:
            trans_mat_group[g_map[g_next], g_map[g_curr]] += 1

    col_sum_p = trans_mat_phase.sum(axis=0)
    col_sum_p[col_sum_p==0] = 1
    trans_mat_phase /= col_sum_p
    
    trans_mat_group = np.log1p(trans_mat_group)
    
    total = sum(stats.values())
    radar = {
        'Sequence Fidelity': stats['Fwd']/total if total>0 else 0,
        'Reverse Replay': stats['Rev']/total if total>0 else 0,
        'Associativity': stats['Jump']/total if total>0 else 0,
        'Jump Precision': 1.0 - (np.mean(jump_dists) if jump_dists else 1.0)
    }
    
    bar_data = {k: v/total if total>0 else 0 for k,v in stats.items()}
    
    return {
        'plot_data': plot_data,
        'phase_matrix': trans_mat_phase,
        'group_matrix': trans_mat_group,
        'radar': radar,
        'bar': bar_data
    }

# ==========================================
# 2. 獨立繪圖函數
# ==========================================

COLOR_MAP = {'Fwd': 'crimson', 'Rev': 'magenta', 'Jump': 'royalblue'}

def add_colorbar(im, ax, label):
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.1)
    cbar = plt.colorbar(im, cax=cax)
    cbar.set_label(label, fontsize=12)
    return cbar

# --- Panel A: Raw Raster ---
def plot_panel_A(results, labels, output_dir):
    fig, axes = plt.subplots(1, 2, figsize=(16, 6))
    
    for i, label in enumerate(labels):
        ax = axes[i]
        data = results[label]['plot_data']
        times = data['times']
        ids = data['ids']
        groups = ids // 10
        phases = ids % 10
        
        scatter = ax.scatter(times, phases, c=groups, cmap='tab20', s=40, alpha=0.9, edgecolors='none')
        ax.plot(times, phases, c='gray', alpha=0.2, lw=0.5)
        
        ax.set_title(f"{label}", fontweight='bold')
        ax.set_ylabel("Circular Phase (0-9)")
        ax.set_xlabel("Time (s)")
        ax.set_yticks(np.arange(10))
        ax.grid(axis='y', alpha=0.3)
        
        add_colorbar(scatter, ax, 'Group ID')
        
    plt.suptitle("A. Raw Replay Sequence", fontweight='bold', fontsize=18, y=1.05)
    plt.tight_layout()
    plt.savefig(f"{output_dir}/Fig3A_Raster.png", dpi=800, bbox_inches='tight')
    plt.close()

# --- Panel B: 2D Trajectory ---
def plot_panel_B(results, labels, output_dir):
    fig, axes = plt.subplots(1, 2, figsize=(18, 8))
    
    for i, label in enumerate(labels):
        ax = axes[i]
        data = results[label]['plot_data']
        times, ids, ends, x_map = data['times'], data['ids'], data['ends'], data['x_map']
        
        groups = ids // 10
        phases = ids % 10
        x = x_map[groups] + np.random.normal(0, 0.5, len(groups)) # Jitter
        
        for xp in x_map:
            ax.scatter(np.full(10, xp), np.arange(10), c='lightgray', s=10, alpha=0.2)
            
        for j in range(len(ids)-1):
            if times[j+1] - ends[j] > 0.2: continue 
            
            g1, p1 = groups[j], phases[j]
            g2, p2 = groups[j+1], phases[j+1]
            
            if g1 == g2:
                if p2 == (p1 + 1) % 10: color = COLOR_MAP['Fwd']
                elif p2 == (p1 - 1) % 10: color = COLOR_MAP['Rev']
                else: color = COLOR_MAP['Jump']
            else:
                color = COLOR_MAP['Jump']
            
            alpha = 0.8 if color != COLOR_MAP['Jump'] else 0.4
            width = 2.0 if color != COLOR_MAP['Jump'] else 1.0
            
            ax.plot([x[j], x[j+1]], [phases[j], phases[j+1]], c=color, alpha=alpha, lw=width)
            
        sc = ax.scatter(x, phases, c=times, cmap='viridis', s=30, zorder=10)
        ax.set_title(f"{label}", fontweight='bold')
        ax.set_ylabel("Circular Phase")
        ax.set_xlabel("Structural Similarity (MDS)")
        ax.set_yticks(np.arange(10))
        ax.set_ylim(-0.5, 9.5)
        
        add_colorbar(sc, ax, 'Time (s)')
        
        custom_lines = [Line2D([0], [0], color=COLOR_MAP['Fwd'], lw=2),
                        Line2D([0], [0], color=COLOR_MAP['Rev'], lw=2),
                        Line2D([0], [0], color=COLOR_MAP['Jump'], lw=1, alpha=0.5)]
        ax.legend(custom_lines, ['Forward', 'Reverse', 'Jump'], loc='upper right', fontsize=10)

    plt.suptitle("B. Replay Trajectory (Navigation)", fontweight='bold', fontsize=18, y=1.02)
    plt.tight_layout()
    plt.savefig(f"{output_dir}/Fig3B_Trajectory.png", dpi=800, bbox_inches='tight')
    plt.close()

# --- Panel C: Phase Matrix (修正報錯) ---
def plot_panel_C(results, labels, output_dir):
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    
    for i, label in enumerate(labels):
        ax = axes[i]
        mat = results[label]['phase_matrix']
        
        # seaborn heatmap 回傳 Axes 物件
        sns.heatmap(mat, ax=ax, cmap="Blues", cbar=False, square=True, vmin=0, vmax=0.6)
        
        ax.set_title(f"{label}", fontweight='bold')
        ax.set_xlabel("Current Phase ($t$)")
        ax.set_ylabel("Next Phase ($t+1$)")
        ax.invert_yaxis()
        ax.set_xticks(np.arange(10)+0.5)
        ax.set_yticks(np.arange(10)+0.5)
        ax.set_xticklabels(np.arange(10))
        ax.set_yticklabels(np.arange(10))
        
        # 🔥 修正點：從 ax.collections[0] 抓取 mappable 物件
        mappable = ax.collections[0]
        add_colorbar(mappable, ax, 'Probability')

    plt.suptitle("C. Transition Matrix (Phase)", fontweight='bold', fontsize=18, y=1.05)
    plt.tight_layout()
    plt.savefig(f"{output_dir}/Fig3C_PhaseMatrix.png", dpi=800, bbox_inches='tight')
    plt.close()

# --- Panel D: Group Matrix (修正報錯) ---
def plot_panel_D(results, labels, output_dir):
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    
    for i, label in enumerate(labels):
        ax = axes[i]
        mat = results[label]['group_matrix']
        
        sns.heatmap(mat, ax=ax, cmap="magma", cbar=False, square=True)
        
        ax.set_title(f"{label}", fontweight='bold')
        ax.set_xlabel("From Group (Similarity Sorted)")
        ax.set_ylabel("To Group (Similarity Sorted)")
        ax.invert_yaxis()
        ax.set_xticks([])
        ax.set_yticks([])
        
        # 🔥 修正點：從 ax.collections[0] 抓取 mappable 物件
        mappable = ax.collections[0]
        add_colorbar(mappable, ax, 'Count (Log)')

    plt.suptitle("D. Group Transition Matrix (Structure)", fontweight='bold', fontsize=18, y=1.05)
    plt.tight_layout()
    plt.savefig(f"{output_dir}/Fig3D_GroupMatrix.png", dpi=800, bbox_inches='tight')
    plt.close()

# --- Panel E & F: Summary Stats ---
def plot_panel_EF(results, labels, output_dir):
    fig = plt.figure(figsize=(16, 7))
    gs = GridSpec(1, 2, width_ratios=[1, 1])
    colors = ['gray', 'crimson'] 
    
    # E. Transition Profile
    ax_bar = fig.add_subplot(gs[0])
    df_list = []
    for label in labels:
        d = results[label]['bar']
        d['Model'] = label
        df_list.append(d)
    df = pd.DataFrame(df_list)
    
    df.set_index('Model')[['Fwd', 'Rev', 'Jump']].plot(
        kind='bar', stacked=True, ax=ax_bar, 
        color=[COLOR_MAP['Fwd'], COLOR_MAP['Rev'], COLOR_MAP['Jump']], 
        width=0.6, legend=False, rot=0
    )
    ax_bar.set_title("E. Transition Profile", fontweight='bold')
    ax_bar.set_ylabel("Proportion")
    ax_bar.set_xlabel("")
    ax_bar.set_ylim(0, 1)
    
    for c in ax_bar.containers:
        labels_txt = [f'{v.get_height():.2f}' if v.get_height() > 0.05 else '' for v in c]
        ax_bar.bar_label(c, labels=labels_txt, label_type='center', color='white', fontsize=12, fontweight='bold')
    
    legend_elements = [Patch(facecolor=COLOR_MAP['Fwd'], label='Forward'),
                       Patch(facecolor=COLOR_MAP['Rev'], label='Reverse'),
                       Patch(facecolor=COLOR_MAP['Jump'], label='Jump')]
    ax_bar.legend(handles=legend_elements, loc='lower center', bbox_to_anchor=(0.5, 1.02), ncol=3, frameon=False)

    # F. Radar Chart
    ax_radar = fig.add_subplot(gs[1], projection='polar')
    categories = list(results[labels[0]]['radar'].keys())
    N = len(categories)
    angles = [n / float(N) * 2 * pi for n in range(N)]
    angles += angles[:1]
    
    for i, label in enumerate(labels):
        values = list(results[label]['radar'].values())
        values += values[:1]
        ax_radar.plot(angles, values, linewidth=3, linestyle='solid', label=label, color=colors[i])
        ax_radar.fill(angles, values, color=colors[i], alpha=0.1)
        
    ax_radar.set_xticks(angles[:-1])
    ax_radar.set_xticklabels(categories, size=11, fontweight='bold')
    ax_radar.set_yticks([0.2, 0.5, 0.8])
    ax_radar.set_yticklabels(["", "", ""], color="grey")
    ax_radar.set_ylim(0, 1)
    ax_radar.set_title("F. Functional Profile", fontweight='bold', y=1.1)
    ax_radar.legend(loc='upper right', bbox_to_anchor=(1.3, 1.1))

    plt.tight_layout()
    plt.savefig(f"{output_dir}/Fig3EF_Stats.png", dpi=800, bbox_inches='tight')
    plt.close()

# ==========================================
# 3. 主程式
# ==========================================
if __name__ == '__main__':
    npat = 1000
    group_size = 10
    start_time = 50.0
    n_plot = 200 
    
    output_dir = "poster_plots/fig3_split/"
    if not os.path.exists(output_dir): os.makedirs(output_dir)

    model_files = {
        "SFA-only": "/flash/FukaiU/pinju-chou/seg-circular/struct_inh_data/lognormal/sfa_hebb_spontaneous1000_activations.h5",
        "dist-STD": "/flash/FukaiU/pinju-chou/dist-seg-circular/struct_inh_data/lognormal/std_hebb_spontaneous_short1000_activations.h5",
    }
    
    model_labels = list(model_files.keys())

    print("Collecting data...")
    results = {}
    for label, path in tqdm(model_files.items()):
        res = get_model_data(path, npat, group_size, start_time, n_plot)
        if res: results[label] = res
        else: print(f"Skipping {label} (No data)")

    if len(results) < 2:
        print("Not enough data.")
        sys.exit()

    print("Plotting Panel A (Raster)...")
    plot_panel_A(results, model_labels, output_dir)
    
    print("Plotting Panel B (Trajectory)...")
    plot_panel_B(results, model_labels, output_dir)
    
    print("Plotting Panel C (Phase Matrix)...")
    plot_panel_C(results, model_labels, output_dir)
    
    print("Plotting Panel D (Group Matrix)...")
    plot_panel_D(results, model_labels, output_dir)
    
    print("Plotting Panel E & F (Stats)...")
    plot_panel_EF(results, model_labels, output_dir)
    
    print("\n✅ All panels generated!")