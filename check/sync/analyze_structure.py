import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
from scipy import signal
import os
import sys
import argparse  # 新增：用於解析命令列參數
from tqdm import tqdm

# --- Path Setup ---
current_dir = os.path.dirname(os.path.abspath(__file__)) 
src_dir = os.path.dirname(current_dir)                   
parent_dir = os.path.dirname(src_dir)                  
utils_path = os.path.join(parent_dir, 'src/')
sys.path.append(utils_path)
from utils import data_path

def analyze_structure(system, namespace, output_dir, start_time, duration, n_sample):
    print(f"Analyzing structure for: {system}")
    print(f"  - Time Window: {start_time}s to {start_time + duration}s")
    print(f"  - Neurons Sampled: {n_sample if n_sample > 0 else 'ALL'}")

    folder = data_path(namespace)
    # 檔名可能需要根據您的實際情況微調 (e.g. spontaneous1000)
    filename = f"{folder}/{system}_spontaneous_short_inhf1000.h5"
    
    if not os.path.exists(filename):
        print(f"❌ File not found: {filename}")
        return

    # 1. Load Data
    with h5py.File(filename, "r") as h5f:
        spikes_exc = h5f['spikes_exc'][:] 
        sim_duration = h5f.attrs['simulation_time']
    
    # Check time bounds
    if start_time >= sim_duration:
        print(f"❌ Start time ({start_time}s) exceeds simulation duration ({sim_duration}s).")
        return
    
    end_time = min(start_time + duration, sim_duration)
    
    # Slice Data by Time
    mask = (spikes_exc[:, 1] >= start_time) & (spikes_exc[:, 1] < end_time)
    spikes = spikes_exc[mask]
    
    if len(spikes) == 0:
        print("❌ No spikes found in the specified time window.")
        return

    times = spikes[:, 1]
    indices = spikes[:, 0].astype(int)
    
    fig, axes = plt.subplots(1, 3, figsize=(20, 5))
    fig.suptitle(f"Structural Analysis: {system} (t={start_time}-{end_time}s)", fontsize=16)

    # --- Sampling Strategy ---
    active_neurons = np.unique(indices)
    total_active = len(active_neurons)
    
    if n_sample > 0 and n_sample < total_active:
        sampled_neurons = np.random.choice(active_neurons, n_sample, replace=False)
        print(f"  - Subsampling {n_sample} from {total_active} active neurons.")
    else:
        sampled_neurons = active_neurons
        print(f"  - Using all {total_active} active neurons (Warning: CCH might be slow).")

    # --- Pre-processing for fast lookup ---
    sort_idx = np.argsort(indices)
    sorted_times = times[sort_idx]
    sorted_indices = indices[sort_idx]
    unique_ids, split_idx = np.unique(sorted_indices, return_index=True)
    grouped_times = np.split(sorted_times, split_idx[1:])
    id_map = {uid: i for i, uid in enumerate(unique_ids)}

    # ==========================================
    # Plot A: Log-ISI Histogram
    # ==========================================
    isis = []
    for nid in sampled_neurons:
        if nid in id_map:
            t = np.sort(grouped_times[id_map[nid]])
            if len(t) > 1:
                diffs = np.diff(t) * 1000 # to ms
                isis.extend(diffs)
    
    sns.histplot(isis, log_scale=True, ax=axes[0], color='teal', bins=50)
    axes[0].set_title("1. Log-ISI Distribution")
    axes[0].set_xlabel("Inter-Spike Interval (ms)")
    axes[0].axvline(10, color='r', linestyle='--', label='Burst (<10ms)')
    axes[0].legend()

    # ==========================================
    # Plot B: Pairwise Cross-Correlogram (CCH)
    # ==========================================
    # CCH 計算量大，如果樣本數 > 500，我們只取前 500 個做 CCH，其他的做 ISI
    cch_sample_size = min(len(sampled_neurons), 500)
    cch_neurons = sampled_neurons[:cch_sample_size]
    
    bin_ms = 0.001 # 1ms bin
    max_lag_ms = 50
    lags = np.arange(-max_lag_ms, max_lag_ms + 1)
    cch_accum = np.zeros(len(lags))
    
    # Binning spikes for correlation
    n_bins = int((end_time - start_time) / bin_ms)
    # Safety check for empty bins
    if n_bins > 0:
        binned_spikes = np.zeros((len(cch_neurons), n_bins))
        
        for i, nid in enumerate(cch_neurons):
            if nid in id_map:
                t = grouped_times[id_map[nid]]
                bin_idx = ((t - start_time) / bin_ms).astype(int)
                bin_idx = bin_idx[(bin_idx >= 0) & (bin_idx < n_bins)]
                binned_spikes[i, bin_idx] = 1

        # Calculate Cross-Corr
        pair_count = 0
        # 只計算部分配對以節省時間 (例如最多算 10000 對)
        max_pairs = 10000
        pairs_calculated = 0
        
        for i in range(len(cch_neurons)):
            for j in range(i+1, len(cch_neurons)):
                corr = signal.correlate(binned_spikes[i], binned_spikes[j], mode='same')
                mid = len(corr) // 2
                center_corr = corr[mid - max_lag_ms : mid + max_lag_ms + 1]
                
                if len(center_corr) == len(cch_accum):
                    cch_accum += center_corr
                    pair_count += 1
                
                pairs_calculated += 1
                if pairs_calculated > max_pairs: break
            if pairs_calculated > max_pairs: break
        
        if pair_count > 0:
            cch_accum /= pair_count
            
        axes[1].plot(lags, cch_accum, color='maroon')
        axes[1].set_title(f"2. Cross-Correlogram (Sync)\nAvg of {pair_count} pairs")
        axes[1].set_xlabel("Lag (ms)")
        axes[1].axvline(0, color='k', linestyle='--', lw=0.5)
    else:
        axes[1].text(0.5, 0.5, "Time window too short", ha='center')

    # ==========================================
    # Plot C: PSD (Ripple)
    # ==========================================
    # PSD 需要足夠的採樣率，這裡用 2ms bin (500Hz)
    psd_bin = 0.002
    psd_bins = np.arange(start_time, end_time, psd_bin)
    if len(psd_bins) > 100: # 至少要有足夠的點
        pop_hist, _ = np.histogram(times, bins=psd_bins)
        pop_rate = pop_hist / (8000 * psd_bin)
        lfp_proxy = pop_rate - np.mean(pop_rate)
        
        freqs, psd = signal.welch(lfp_proxy, fs=1/psd_bin, nperseg=min(1024, len(lfp_proxy)))
        
        axes[2].semilogy(freqs, psd, color='purple')
        axes[2].set_title("3. Power Spectrum (Ripple)")
        axes[2].set_xlabel("Frequency (Hz)")
        axes[2].set_xlim(0, 300)
        axes[2].axvspan(150, 250, color='yellow', alpha=0.3, label='Ripple Band')
        axes[2].legend()
    else:
        axes[2].text(0.5, 0.5, "Duration too short for PSD", ha='center')

    plt.tight_layout()
    save_path = os.path.join(output_dir, f"{system}_structure.png")
    plt.savefig(save_path)
    plt.close(fig)
    print(f"✅ Saved to {save_path}")

if __name__ == '__main__':
    # Argument Parsing
    parser = argparse.ArgumentParser(description="Analyze network structure (ISI, CCH, PSD)")
    parser.add_argument('--namespace', type=str, default='lognormal', help="Data folder name")
    parser.add_argument('--start', type=float, default=100.0, help="Start time for analysis (s)")
    parser.add_argument('--duration', type=float, default=10.0, help="Duration to analyze (s)")
    parser.add_argument('--n_sample', type=int, default=500, help="Number of neurons to sample (-1 for all)")
    
    args = parser.parse_args()
    
    output_dir = 'log_norm-STD_inhf/check/structure_check/'
    os.makedirs(output_dir, exist_ok=True)
    
    systems = ['hebb', 'sfa_hebb'] # 您可以根據需要修改這個列表
    
    for sys in tqdm(systems):
        analyze_structure(sys, args.namespace, output_dir, args.start, args.duration, args.n_sample)