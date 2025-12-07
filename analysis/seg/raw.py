import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import os
import sys
import argparse
from tqdm import tqdm

# 設定繪圖風格
sns.set_context("talk")
plt.rcParams.update({'font.size': 14, 'axes.linewidth': 2})

# --- Path Setup ---
current_dir = os.path.dirname(os.path.abspath(__file__)) 
src_dir = os.path.dirname(current_dir)                   
parent_dir = os.path.dirname(src_dir)                  
utils_path = os.path.join(parent_dir, 'src/')
sys.path.append(utils_path)
from utils import data_path

def analyze_raw_circular(system, namespace, output_dir, start_time, npat=1000, group_size=10):
    print(f"Analyzing Raw Circular Data for: {system}")
    print(f"  - Start Time: {start_time}s")
    
    folder = data_path(namespace)
    filename = f"{folder}/{system}_spontaneous_short{npat}_activations.h5"
    
    if not os.path.exists(filename):
        print(f"❌ File not found: {filename}")
        return

    # 1. 讀取 Raw Events
    with h5py.File(filename, "r") as h5f:
        if 'activations' not in h5f: return
        events = h5f['activations'][:] # [times, durations, ids]
        
    # 按照時間排序
    sort_idx = np.argsort(events[0])
    all_times = events[0][sort_idx] * 0.01 # 轉秒
    all_ids = events[2][sort_idx].astype(int)
    
    # 2. 時間過濾 (Time Slicing)
    # 只保留 start_time 之後的事件
    mask = all_times >= start_time
    event_times = all_times[mask]
    event_ids = all_ids[mask]
    
    if len(event_times) == 0:
        print("⚠️ No events found after start_time.")
        return

    print(f"  - Found {len(event_times)} events after t={start_time}s")

    # 3. 解析 Group 和 Phase
    phases = event_ids % group_size       # 0 ~ 9
    groups = event_ids // group_size      # 0 ~ 99
    
    # ------------------------------------------------
    # Plot 1: Phase Raster (Visual Check)
    # ------------------------------------------------
    # 為了看清楚結構，我們只畫出過濾後的前 N 個事件
    n_plot = min(300, len(event_ids))
    
    fig1, ax1 = plt.subplots(figsize=(14, 6))
    
    # 使用 tab20 色盤區分不同 Group
    scatter = ax1.scatter(event_times[:n_plot], phases[:n_plot], 
                          c=groups[:n_plot], cmap='tab20', s=80, alpha=0.9, edgecolors='k')
    
    # 畫連接線
    ax1.plot(event_times[:n_plot], phases[:n_plot], c='gray', alpha=0.3, lw=1)

    ax1.set_title(f"Raw Replay Sequence: {system} (from t={start_time}s)\n(Color = Different Groups)", fontweight='bold')
    ax1.set_xlabel("Time (s)")
    ax1.set_ylabel("Circular Phase (0 $\\to$ 9)")
    ax1.set_yticks(np.arange(group_size))
    ax1.grid(True, axis='y', linestyle='--', alpha=0.5)
    
    # Colorbar
    cbar = plt.colorbar(scatter, ax=ax1)
    cbar.set_label('Group ID')

    save_path1 = os.path.join(output_dir, f"{system}_raw_raster_start{int(start_time)}.png")
    plt.savefig(save_path1)
    print(f"✅ Phase Raster saved: {save_path1}")

    # ------------------------------------------------
    # Plot 2: Transition Matrix (Statistics) [修正版]
    # ------------------------------------------------
    
    trans_matrix = np.zeros((group_size, group_size))
    valid_transitions = 0
    
    # 設定最大允許間隔 (與 2D plot 保持一致)
    # 建議設為 0.2s (200ms) 或 0.5s，視您的 replay duration 而定
    max_gap = 0.2 
    
    # 計算每個事件的結束時間
    # events[1] 是 duration steps (假設 dt=0.01s)
    event_durations = all_durs[mask] * 0.01 
    event_ends = event_times + event_durations
    
    for i in range(len(event_ids) - 1):
        curr_g = groups[i]
        next_g = groups[i+1]
        
        # 時間檢查
        curr_end = event_ends[i]
        next_start = event_times[i+1]
        gap = next_start - curr_end
        
        # 邏輯 1: 如果間隔太久，視為斷開，不計入轉移矩陣
        if gap > max_gap:
            continue
            
        # 邏輯 2: 如果重疊 (gap < 0)，我們依然計入 (視為快速轉移)
        
        # 邏輯 3: 只統計 Group 沒變的情況 (Within-group sequence)
        if curr_g == next_g:
            curr_p = phases[i]
            next_p = phases[i+1]
            trans_matrix[next_p, curr_p] += 1
            valid_transitions += 1
            
    # Normalize (Column stochastic)
    col_sums = trans_matrix.sum(axis=0)
    # 避免除以 0
    col_sums[col_sums == 0] = 1 
    trans_matrix_prob = trans_matrix / col_sums

    fig2, ax2 = plt.subplots(figsize=(8, 7))
    sns.heatmap(trans_matrix_prob, annot=True, fmt=".2f", cmap="Blues", square=True, cbar=True, ax=ax2)
    
    ax2.set_title(f"Transition Prob (t>{start_time}s)\nValid transitions: {valid_transitions}", fontweight='bold')
    ax2.set_xlabel("Current Phase ($t$)")
    ax2.set_ylabel("Next Phase ($t+1$)")
    ax2.invert_yaxis() 
    
    save_path2 = os.path.join(output_dir, f"{system}_transition_matrix_start{int(start_time)}.png")
    plt.savefig(save_path2)
    print(f"✅ Transition Matrix saved: {save_path2}")
    
    plt.close('all')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--namespace', type=str, default='lognormal')
    parser.add_argument('--start', type=float, default=10.0, help="Skip events before this time (seconds)")
    
    args = parser.parse_args()
    
    output_dir = 'dist-seg-circular/circular_raw/'
    os.makedirs(output_dir, exist_ok=True)
    
    # 填入您要分析的模型列表
    systems = ['std_hebb', 'mix_hebb'] 
    
    for sys in tqdm(systems):
        analyze_raw_circular(sys, args.namespace, output_dir, args.start)