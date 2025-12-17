import numpy as np
import h5py
import matplotlib.pyplot as plt
import seaborn as sns
import sys
import os
import argparse
from tqdm import tqdm

# --- 1. Path Setup ---
current_dir = os.path.dirname(os.path.abspath(__file__)) 
src_dir = os.path.dirname(current_dir)                   
parent_dir = os.path.dirname(src_dir)                  
utils_path = os.path.join(parent_dir, 'src/')

sys.path.append(utils_path)
from utils import data_path

# --- 2. Logic to load activations (With Time Slicing) ---
def load_activations(system, npat, run_name, namespace, start_time=0, duration=None):
    """
    Load activation traces and filter by time window.
    Assumes time resolution (dt) is 10ms (0.01s).
    """
    folder = data_path(namespace)
    filename = f"{folder}/{system}_{run_name}{npat}_activations.h5"

    if not os.path.exists(filename):
        print(f"Warning: File not found: {filename}")
        return None, None

    with h5py.File(filename, "r") as h5f:
        # traces shape: (n_patterns, n_time_steps)
        # Load all first, then slice (to avoid complex HDF5 slicing if possible)
        traces_all = h5f['traces'][:] 
        
        if 'activations' in h5f:
            sparse_events_all = h5f['activations'][:]
        else:
            sparse_events_all = None

    # --- Time Slicing Logic ---
    dt = 0.01 # 10ms resolution
    total_steps = traces_all.shape[1]
    
    # Calculate indices
    start_idx = int(start_time / dt)
    
    if duration is not None:
        end_idx = int((start_time + duration) / dt)
        end_idx = min(end_idx, total_steps)
    else:
        end_idx = total_steps

    # Check bounds
    if start_idx >= total_steps:
        print(f"Error: Start time {start_time}s is beyond simulation length.")
        return None, None

    # Slice Traces
    traces = traces_all[:, start_idx:end_idx]

    # Filter Sparse Events
    # sparse_events structure: [0]=time_index, [1]=duration_steps, [2]=pattern_ix
    sparse_events = None
    if sparse_events_all is not None:
        event_times = sparse_events_all[0]
        # Keep events that start within the window
        mask = (event_times >= start_idx) & (event_times < end_idx)
        sparse_events = sparse_events_all[:, mask]

    return traces, sparse_events

# --- 3. Analysis & Plotting Function ---
def diagnose_and_save(traces, sparse_events, system, output_dir, time_info_str):
    """
    Perform health check metrics and save plot to output_dir
    """
    # Metric A: Co-activation (Synchrony)
    active_binary = (traces > 0.5).astype(int)
    co_activation_counts = np.sum(active_binary, axis=0)
    
    if co_activation_counts.size == 0:
        max_sync = 0
        mean_sync = 0
    else:
        max_sync = np.max(co_activation_counts)
        mean_sync = np.mean(co_activation_counts)
    
    # Metric B: Correlation (Independence)
    n_sample = min(traces.shape[0], 500)
    if n_sample > 1 and traces.shape[1] > 1:
        corr_matrix = np.corrcoef(traces[:n_sample])
        # Handle NaN if constant activity
        corr_matrix = np.nan_to_num(corr_matrix)
        avg_corr = np.mean(corr_matrix[np.triu_indices(n_sample, k=1)])
    else:
        avg_corr = 0
        corr_matrix = np.zeros((n_sample, n_sample))

    # Metric C: Duration (Replay Quality)
    if sparse_events is not None and sparse_events.shape[1] > 0:
        durations_ms = sparse_events[1] * 10 
        avg_dur = np.mean(durations_ms)
    else:
        durations_ms = []
        avg_dur = 0

    # --- Plotting ---
    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    fig.suptitle(f"Network Health Check: {system}\n({time_info_str})", fontsize=14)
    
    # Plot 1: Co-activation (Log Scale)
    sns.histplot(co_activation_counts, binwidth=5, ax=axes[0], kde=False, log_scale=(False, True))
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
    plt.close(fig) 
    
    return {
        "max_sync": max_sync,
        "avg_corr": avg_corr,
        "avg_dur": avg_dur
    }

if __name__ == '__main__':
    # --- Arguments ---
    parser = argparse.ArgumentParser(description="Analyze pattern synchrony and health.")
    parser.add_argument('--start', type=float, default=10.0, help="Start time for analysis (seconds)")
    parser.add_argument('--duration', type=float, default=None, help="Duration to analyze (seconds). If None, analyze until end.")
    parser.add_argument('--namespace', type=str, default='lognormal', help="Data folder namespace")
    parser.add_argument('--run_name', type=str, default='spontaneous', help="Run name suffix (e.g. spontaneous)")
    parser.add_argument('--npat', type=int, default=1000, help="Number of patterns")
    
    args = parser.parse_args()

    # --- Configuration ---
    output_dir = 'log_norm-STD_inhf/check/sync_check/'
    os.makedirs(output_dir, exist_ok=True)
    
    # 定義要跑的系統列表
    systems = ['hebb', 'sfa_hebb']
    # systems = ['hebb', 'rate', 'hebb_smooth_rate', 'sfa_hebb', 'sfa_rate', 'sfa_hebb_smooth_rate']
    
    time_info = f"t={args.start}s"
    if args.duration:
        time_info += f" to {args.start + args.duration}s"
    else:
        time_info += " to end"

    print(f"Starting Health Check for {len(systems)} systems...")
    print(f"  - Namespace: {args.namespace}")
    print(f"  - Time Window: {time_info}")
    print(f"  - Saving plots to: {output_dir}")
    print("-" * 75)
    print(f"{'System':<20} | {'Max Sync':<10} | {'Avg Corr':<10} | {'Avg Dur (ms)':<15} | {'Status'}")
    print("-" * 75)

    for system in tqdm(systems):
        # 1. Load with Time Slicing
        traces, sparse_events = load_activations(
            system, 
            args.npat, 
            args.run_name, 
            args.namespace, 
            start_time=args.start, 
            duration=args.duration
        )
        
        if traces is None:
            continue
            
        # 2. Analyze & Plot
        metrics = diagnose_and_save(traces, sparse_events, system, output_dir, time_info)
        
        # 3. Report
        status = "✅ OK"
        if metrics['max_sync'] > (args.npat * 0.3): status = "⚠️ High Sync"
        if metrics['avg_corr'] > 0.2: status = "⚠️ High Corr"
        
        print(f"{system:<20} | {metrics['max_sync']:<10} | {metrics['avg_corr']:.4f}     | {metrics['avg_dur']:.1f}            | {status}")

    print("-" * 75)
    print("Done.")