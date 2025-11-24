import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import sys
import os
from tqdm import tqdm

# --- 1. Path Setup (Same as rasters.py) ---
current_dir = os.path.dirname(os.path.abspath(__file__)) 
src_dir = os.path.dirname(current_dir)                   
parent_dir = os.path.dirname(src_dir)                  
utils_path = os.path.join(parent_dir, 'src/')

sys.path.append(utils_path)
from utils import data_path

# --- 2. Logic to load activations ---
def load_activations(system, npat, run_name, namespace):
    """
    Load activation traces from the output of get_activations.py
    Target file: {system}_{run_name}{npat}_activations.h5
    """
    folder = data_path(namespace)
    # 組合檔名，對應 get_activations.py 的輸出格式
    filename = f"{folder}/{system}_{run_name}{npat}_activations.h5"

    if not os.path.exists(filename):
        print(f"Warning: File not found: {filename}")
        return None, None

    with h5py.File(filename, "r") as h5f:
        # traces shape: (n_patterns, n_time_steps)
        traces = h5f['traces'][:] 
        # sparse events: [times, durations, pattern_ixs]
        # 注意：有些版本的 get_activations 可能存成 'activations' dataset
        if 'activations' in h5f:
            sparse_events = h5f['activations'][:]
        else:
            sparse_events = None
            
    return traces, sparse_events

# --- 3. Analysis & Plotting Function ---
def diagnose_and_save(traces, sparse_events, system, output_dir):
    """
    Perform health check metrics and save plot to output_dir
    """
    # Metric A: Co-activation (Synchrony)
    # 判定 pattern 活化的閾值，這裡設為 0.5 (假設 trace 是 normalized fraction)
    active_binary = (traces > 0.5).astype(int)
    co_activation_counts = np.sum(active_binary, axis=0)
    max_sync = np.max(co_activation_counts)
    mean_sync = np.mean(co_activation_counts)
    
    # Metric B: Correlation (Independence)
    # 只取前 500 個 pattern 算相關性矩陣以節省時間
    n_sample = min(traces.shape[0], 500)
    if n_sample > 1:
        corr_matrix = np.corrcoef(traces[:n_sample])
        # 取上三角矩陣平均 (不含對角線)
        avg_corr = np.mean(corr_matrix[np.triu_indices(n_sample, k=1)])
    else:
        avg_corr = 0
        corr_matrix = np.zeros((n_sample, n_sample))

    # Metric C: Duration (Replay Quality)
    if sparse_events is not None and sparse_events.shape[1] > 0:
        # 假設 get_activations 用 10 phase sharding, 每個 bin 代表 10ms
        durations_ms = sparse_events[1] * 10 
        avg_dur = np.mean(durations_ms)
    else:
        durations_ms = []
        avg_dur = 0

    # --- Plotting ---
    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    fig.suptitle(f"Network Health Check: {system}", fontsize=16)
    
    # Plot 1: Co-activation (Log Scale)
    # sns.histplot(co_activation_counts, bins=30, ax=axes[0], kde=False, log_scale=(False, True))
    sns.histplot(co_activation_counts, binwidth=5, ax=axes[0], kde=False, log_scale=(False, True))
    print(co_activation_counts)
    axes[0].set_xlim(0, 150)
    axes[0].set_title(f"Co-activation (Sync)\nMax: {max_sync} patterns")
    axes[0].set_xlabel("# Simultaneous Active Patterns")
    axes[0].set_ylabel("Count (Log Scale)")
    
    # Plot 2: Duration
    if len(durations_ms) > 0:
        sns.histplot(durations_ms, bins=30, ax=axes[1], color='orange')
        axes[1].axvline(50, color='r', linestyle='--', alpha=0.5, label='Healthy Min (50ms)')
        axes[1].axvline(200, color='r', linestyle='--', alpha=0.5, label='Healthy Max (200ms)')
        axes[1].legend()
    axes[1].set_title(f"Replay Duration\nMean: {avg_dur:.1f} ms")
    axes[1].set_xlabel("Duration (ms)")

    # Plot 3: Correlation
    sns.heatmap(corr_matrix, vmin=-1, vmax=1, cmap="coolwarm", ax=axes[2], cbar=False)
    axes[2].set_title(f"Pattern Correlations\nAvg Corr: {avg_corr:.4f}")

    plt.tight_layout()
    
    # Save figure
    save_path = os.path.join(output_dir, f"{system}_health_check.png")
    plt.savefig(save_path)
    plt.close(fig) # Close to free memory
    
    return {
        "max_sync": max_sync,
        "avg_corr": avg_corr,
        "avg_dur": avg_dur
    }

if __name__ == '__main__':
    # --- Configuration ---
    # 設定要輸出的資料夾，類似 rasters.py 的 output_dir
    output_dir = 'dist-STD/plotting/data/sync_check/'
    os.makedirs(output_dir, exist_ok=True)
    
    namespace = 'lognormal'
    run_name = 'spontaneous' # 根據您的檔案命名習慣調整 (例如 rasters.py 裡是寫死的)
    npat = 1000
    
    # 定義要跑的系統列表 (與 rasters.py 保持一致)
    systems = ['hebb_smooth_rate']
    # systems = ['hebb', 'rate', 'hebb_smooth_rate', 'sfa_hebb', 'sfa_rate', 'sfa_hebb_smooth_rate']
    
    print(f"Starting Health Check for {len(systems)} systems...")
    print(f"Reading from: {data_path(namespace)}")
    print(f"Saving plots to: {output_dir}")
    print("-" * 60)
    print(f"{'System':<20} | {'Max Sync':<10} | {'Avg Corr':<10} | {'Avg Dur (ms)':<15} | {'Status'}")
    print("-" * 60)

    for system in tqdm(systems):
        # 1. Load
        traces, sparse_events = load_activations(system, npat, run_name, namespace)
        
        if traces is None:
            continue
            
        # 2. Analyze & Plot
        metrics = diagnose_and_save(traces, sparse_events, system, output_dir)
        
        # 3. Simple Console Report
        # 簡單的狀態判斷邏輯
        status = "✅ OK"
        if metrics['max_sync'] > (npat * 0.3): status = "⚠️ High Sync"
        if metrics['avg_corr'] > 0.2: status = "⚠️ High Corr"
        
        print(f"{system:<20} | {metrics['max_sync']:<10} | {metrics['avg_corr']:.4f}     | {metrics['avg_dur']:.1f}            | {status}")

    print("-" * 60)
    print("Done.")