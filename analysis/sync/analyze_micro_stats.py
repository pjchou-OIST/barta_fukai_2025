import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import os
import sys
from tqdm import tqdm

# --- 1. Path Setup (Same as rasters.py) ---
current_dir = os.path.dirname(os.path.abspath(__file__)) 
src_dir = os.path.dirname(current_dir)                   
parent_dir = os.path.dirname(src_dir)                  
utils_path = os.path.join(parent_dir, 'src/')

sys.path.append(utils_path)
from utils import data_path

# --- 2. Data Loading & Calculation ---
def analyze_system_stats(system, namespace, output_dir, record_type='spontaneous'):
    folder = data_path(namespace)
    # 這裡假設您的檔名格式是 {system}_spontaneous.h5 (或是 spontaneous1000.h5，請根據實際情況調整)
    # 如果您的檔名有數字 (e.g. spontaneous1000)，請在這裡修改 filename
    filename = f"{folder}/{system}_{record_type}1000.h5" 
    
    if not os.path.exists(filename):
        print(f"Skipping {system}: File not found at {filename}")
        return

    print(f"Analyzing {system}...")
    
    with h5py.File(filename, "r") as h5f:
        # 讀取 exc spikes: (2, N_spikes) -> [indices, times]
        # 我們只讀取興奮性神經元 (Excitatory)，因為它們是主要的資訊攜帶者
        spikes_exc = h5f['spikes_exc'][:] 
        duration = h5f.attrs['simulation_time']
    
    indices = spikes_exc[:, 0].astype(int)
    times = spikes_exc[:, 1]
    
    # --- A. Firing Rates (Hz) ---
    # 假設 N_exc = 8000
    n_exc = 8000
    spike_counts = np.bincount(indices, minlength=n_exc)
    rates = spike_counts / duration
    
    # 過濾掉完全不發火的神經元 (為了 Log plot 美觀)
    active_rates = rates[rates > 0]

    # --- B. CV of ISI (Coefficient of Variation) [高速優化版] ---
    # CV = std(ISI) / mean(ISI)
    
    print("  Calculating CVs (Optimized)...")
    
    # 1. 先根據神經元 ID 對所有數據進行排序
    # 這一步讓相同的神經元排在一起，避免重複掃描
    sort_order = np.argsort(indices)
    sorted_indices = indices[sort_order]
    sorted_times = times[sort_order]
    
    # 2. 找出每個神經元的切分點 (Split points)
    # unique_neurons 是有發火的神經元 ID
    # split_idx 是它們在陣列中的起始位置
    unique_neurons, split_idx = np.unique(sorted_indices, return_index=True)
    
    # 3. 切割成 List of Arrays (每個元素是一個神經元的所有發火時間)
    grouped_spikes = np.split(sorted_times, split_idx[1:])
    
    cvs = []
    
    # 4. 快速迭代 (現在不需要 search 了，直接拿來算)
    # 為了省時，我們只算那些發火次數 > 10 的
    # 因為 unique_neurons 和 grouped_spikes 是對應的，我們用 zip
    
    # 為了避免計算太多，如果有超過 2000 個神經元，我們隨機抽樣
    target_count = 0
    max_samples = 2000 
    
    # 建立一個隨機遮罩來抽樣 (如果神經元太多的話)
    if len(grouped_spikes) > max_samples:
        sample_mask = np.random.rand(len(grouped_spikes)) < (max_samples / len(grouped_spikes))
    else:
        sample_mask = np.ones(len(grouped_spikes), dtype=bool)

    for i, neuron_spikes in enumerate(grouped_spikes):
        # 簡單過濾：只算被抽樣到的 & 發火次數夠多的
        if not sample_mask[i] or len(neuron_spikes) < 10:
            continue
            
        # 因為已經 sort 過 index，但 time 可能是亂的 (如果 Brian2 輸出是按時間排序，這裡可能亂掉)
        # 所以對時間做一次 sort (這很快，因為單一神經元 spike 不多)
        neuron_spikes = np.sort(neuron_spikes)
        
        isis = np.diff(neuron_spikes)
        if len(isis) > 1:
            mean_isi = np.mean(isis)
            std_isi = np.std(isis)
            if mean_isi > 1e-9: # 避免除以 0
                cvs.append(std_isi / mean_isi)

    cvs = np.array(cvs)
    print(f"  Calculated CVs for {len(cvs)} neurons.")

    # --- C. Population Rate (PSTH) ---
    # 計算全網平均放電率隨時間的變化 (Bin = 20ms)
    time_bins = np.arange(0, duration, 0.02) # 20ms bins
    pop_hist, _ = np.histogram(times, bins=time_bins)
    # 換算成 Hz: count / (N_neurons * bin_width)
    pop_rate_hz = pop_hist / (n_exc * 0.02)
    
    # --- Plotting ---
    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    fig.suptitle(f"Micro-stats Analysis: {system} ({namespace})", fontsize=16)

    # Plot 1: Firing Rate Distribution (Log Scale)
    # 這應該要呈現 Log-normal (鐘形曲線)
    sns.histplot(active_rates, log_scale=True, ax=axes[0], kde=True, color='blue')
    axes[0].set_title("Firing Rate Distribution")
    axes[0].set_xlabel("Rate (Hz) [Log Scale]")
    axes[0].set_ylabel("Count")
    
    # Plot 2: CV of ISI Distribution
    # 標示出 1.0 的位置
    sns.histplot(cvs, ax=axes[1], kde=True, color='green')
    axes[1].axvline(1.0, color='r', linestyle='--', label='Poisson (1.0)')
    axes[1].set_title(f"Irregularity (CV of ISI)\nMean CV: {np.mean(cvs):.2f}")
    axes[1].set_xlabel("CV (std/mean)")
    axes[1].legend()

    # Plot 3: Population Rate (First 5 seconds zoom-in)
    # 只畫前 5 秒，不然太密看不清楚
    zoom_time = min(5.0, duration)
    zoom_bins = int(zoom_time / 0.02)
    
    axes[2].plot(time_bins[:zoom_bins], pop_rate_hz[:zoom_bins], color='black', lw=1)
    axes[2].set_title("Population Rate (First 5s)")
    axes[2].set_xlabel("Time (s)")
    axes[2].set_ylabel("Mean Rate (Hz)")
    
    plt.tight_layout()
    
    # Save figure
    save_path = os.path.join(output_dir, f"{system}_micro_stats.png")
    plt.savefig(save_path)
    plt.close(fig)
    print(f"Saved plot to {save_path}")


if __name__ == '__main__':
    # --- Configuration ---
    
    # 1. Namespace: 指定要分析的資料夾 (e.g., 'lognormal' 或 'lognormal_dist_std')
    # 您可以在這裡切換，一次分析一個 folder
    namespace = 'lognormal' 
    
    # 2. Output Directory: 圖片存檔位置
    # 建議分開存，以免混淆 (e.g., micro_stats/homogeneous/ 或 micro_stats/heterogeneous/)
    output_dir = os.path.join(parent_dir, 'log_norm-STD/micro_stats/')
    os.makedirs(output_dir, exist_ok=True)
    
    # 3. Systems to analyze: 在這裡列出所有您想跑的模型
    systems = [
        # 'hebb', 
        'hebb_long'
        # 'rate', 
        # 'hebb_smooth_rate',
        # 'sfa_hebb',
        # 'sfa_rate',
        # 'sfa_hebb_smooth_rate'
    ]

    print(f"Starting Micro-stats Analysis for namespace: {namespace}")
    print(f"Output directory: {output_dir}")
    print("-" * 60)

    for system in tqdm(systems):
        analyze_system_stats(system, namespace, output_dir, 'spontaneous_short')
        
    print("Done.")