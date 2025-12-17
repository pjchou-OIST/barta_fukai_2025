import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import os
import sys
from tqdm import tqdm
import argparse # <-- 新增 argparse 模組

# --- 1. Path Setup (Same as rasters.py) ---
current_dir = os.path.dirname(os.path.abspath(__file__)) 
src_dir = os.path.dirname(current_dir)                   
parent_dir = os.path.dirname(src_dir)                  
utils_path = os.path.join(parent_dir, 'src/')

sys.path.append(utils_path)
from utils import data_path

# --- 2. Data Loading & Calculation ---
# 函式定義新增了 start_time 和 end_time 參數
def analyze_system_stats(system, namespace, output_dir, start_time, end_time):
    folder = data_path(namespace)
    # 這裡假設您的檔名格式是 {system}_spontaneous.h5 (或是 spontaneous1000.h5，請根據實際情況調整)
    filename = f"{folder}/{system}_spontaneous_short_inhf1000.h5" 
    
    if not os.path.exists(filename):
        print(f"Skipping {system}: File not found at {filename}")
        return

    print(f"Analyzing {system}...")
    
    with h5py.File(filename, "r") as h5f:
        # 讀取 exc spikes: (2, N_spikes) -> [indices, times]
        spikes_exc = h5f['spikes_exc'][:] 
        duration = h5f.attrs['simulation_time']
    
    # --- 關鍵修改點：根據 start_time 和 end_time 篩選數據 ---
    
    # 確保時間在合理的範圍內 (start < end)
    if start_time >= end_time:
        print("  Error: start_time must be less than end_time.")
        return
        
    # 限制分析時間在模擬時間內
    analysis_end_time = min(end_time, duration)
    analysis_start_time = max(start_time, 0.0)
    
    # 計算實際分析的時長 (Duration of analysis)
    analysis_duration = analysis_end_time - analysis_start_time
    
    print(f"  Analysis time window: [{analysis_start_time:.2f}s, {analysis_end_time:.2f}s], Duration: {analysis_duration:.2f}s")
    
    if analysis_duration <= 0:
        print("  Analysis duration is zero or negative after clipping. Skipping.")
        return
        
    # 篩選 spikes
    times = spikes_exc[:, 1]
    time_mask = (times >= analysis_start_time) & (times < analysis_end_time)
    
    indices = spikes_exc[time_mask, 0].astype(int)
    times = spikes_exc[time_mask, 1]
    
    # 如果篩選後沒有 spikes，則跳過
    if len(indices) == 0:
        print("  No spikes found in the specified time window. Skipping.")
        return
        
    # --- A. Firing Rates (Hz) ---
    # 假設 N_exc = 8000
    n_exc = 8000
    spike_counts = np.bincount(indices, minlength=n_exc)
    # 使用 analysis_duration 來計算 Rate
    rates = spike_counts / analysis_duration
    
    # 過濾掉完全不發火的神經元
    active_rates = rates[rates > 0]

    # --- B. CV of ISI (Coefficient of Variation) ---
    # CV = std(ISI) / mean(ISI)
    
    print("  Calculating CVs (Optimized)...")
    
    # 1. 先根據神經元 ID 對所有數據進行排序
    sort_order = np.argsort(indices)
    sorted_indices = indices[sort_order]
    sorted_times = times[sort_order]
    
    # 2. 找出每個神經元的切分點 (Split points)
    unique_neurons, split_idx = np.unique(sorted_indices, return_index=True)
    
    # 3. 切割成 List of Arrays (每個元素是一個神經元的所有發火時間)
    grouped_spikes = np.split(sorted_times, split_idx[1:])
    
    cvs = []
    
    # 4. 快速迭代 (與原程式碼相同，使用抽樣和發火次數過濾)
    target_count = 0
    max_samples = 2000 
    
    if len(grouped_spikes) > max_samples:
        sample_mask = np.random.rand(len(grouped_spikes)) < (max_samples / len(grouped_spikes))
    else:
        sample_mask = np.ones(len(grouped_spikes), dtype=bool)

    for i, neuron_spikes in enumerate(grouped_spikes):
        if not sample_mask[i] or len(neuron_spikes) < 10:
            continue
            
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
    # 使用新的時間範圍來建立 bin
    time_bins = np.arange(analysis_start_time, analysis_end_time + 1e-6, 0.02) # 20ms bins
    pop_hist, _ = np.histogram(times, bins=time_bins)
    # 換算成 Hz: count / (N_neurons * bin_width)
    pop_rate_hz = pop_hist / (n_exc * 0.02)
    
    # Plotting:
    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    fig.suptitle(f"Micro-stats Analysis: {system} ({namespace}) [Time: {analysis_start_time:.2f}s - {analysis_end_time:.2f}s]", fontsize=14)

    # Plot 1: Firing Rate Distribution (Log Scale)
    sns.histplot(active_rates, log_scale=True, ax=axes[0], kde=True, color='blue')
    axes[0].set_title("Firing Rate Distribution")
    axes[0].set_xlabel("Rate (Hz) [Log Scale]")
    axes[0].set_ylabel("Count")
    
    # Plot 2: CV of ISI Distribution
    sns.histplot(cvs, ax=axes[1], kde=True, color='green')
    axes[1].axvline(1.0, color='r', linestyle='--', label='Poisson (1.0)')
    axes[1].set_title(f"Irregularity (CV of ISI)\nMean CV: {np.mean(cvs):.2f}")
    axes[1].set_xlabel("CV (std/mean)")
    axes[1].legend()

    # Plot 3: Population Rate (Time Series)
    # 畫出整個分析時間範圍的 Population Rate
    axes[2].plot(time_bins[:-1], pop_rate_hz, color='black', lw=1)
    axes[2].set_title(f"Population Rate ({analysis_start_time:.1f}s - {analysis_end_time:.1f}s)")
    axes[2].set_xlabel("Time (s)")
    axes[2].set_ylabel("Mean Rate (Hz)")
    
    plt.tight_layout()
    
    # Save figure
    # 檔名也加入時間資訊以區分
    save_path = os.path.join(output_dir, f"{system}_micro_stats_{int(analysis_start_time)}s_to_{int(analysis_end_time)}s.png")
    plt.savefig(save_path)
    plt.close(fig)
    print(f"Saved plot to {save_path}")


if __name__ == '__main__':
    # --- A. Argument Parsing (新增的區塊) ---
    parser = argparse.ArgumentParser(description="Analyze micro-stats (Firing Rate, CV, Pop. Rate) for simulation data within a specified time window.")
    
    # 定義 start_time 參數
    parser.add_argument(
        '--start_time', 
        type=float, 
        default=10.0, # 預設從 1.0 秒開始分析 (跳過啟動階段)
        help='Analysis start time in seconds (default: 1.0)'
    )
    
    # 定義 end_time 參數
    parser.add_argument(
        '--end_time', 
        type=float, 
        default=15.0, # 預設到 5.0 秒結束
        help='Analysis end time in seconds (default: 5.0). Use a large number (e.g., 9999.0) to analyze full duration.'
    )
    
    args = parser.parse_args()
    
    # --- B. Configuration (使用 args 參數) ---
    
    # 1. Namespace: 指定要分析的資料夾
    namespace = 'lognormal' 
    
    # 2. Output Directory: 圖片存檔位置
    output_dir = os.path.join(parent_dir, 'log_norm-STD_inhf/check/micro_stats/')
    os.makedirs(output_dir, exist_ok=True)
    
    # 3. Systems to analyze: 在這裡列出所有您想跑的模型
    systems = [
        'hebb', 
        # 'rate', 
        # 'hebb_smooth_rate',
        'sfa_hebb',
        # 'sfa_rate',
        # 'sfa_hebb_smooth_rate'
    ]

    print(f"Starting Micro-stats Analysis for namespace: {namespace}")
    print(f"Analysis Time: {args.start_time:.2f}s to {args.end_time:.2f}s")
    print(f"Output directory: {output_dir}")
    print("-" * 60)

    for system in tqdm(systems):
        # system = f'{system}_550'
        # 呼叫函式時，傳入新的 start_time 和 end_time 參數
        analyze_system_stats(system, namespace, output_dir, args.start_time, args.end_time)
        
    print("Done.")