import h5py
import numpy as np
import matplotlib.pyplot as plt
import argparse
import sys
import os

# --- 路徑設定 (與原檔案相同) ---
current_dir = os.path.dirname(os.path.abspath(__file__)) # -> /my_project/src/app
src_dir = os.path.dirname(current_dir)                    # -> /my_project/src
utils_path = os.path.join(src_dir, 'src/')

sys.path.append(utils_path)
from utils import data_path

def find_segmented_chains(events, time_window, min_length, chain_length, npat):
    """
    使用 "修剪-延續-種植" 的單次掃描演算法，找出所有基於時間的鏈。
    
    *** 新規則: 鏈不允許跨越 segment 邊界 ***
    (例如，如果 chain_length=10, 則 9->10 或 10->9 不是有效延續)
    
    :param events: 已排序的 (time, pattern_id) 元組列表
    :param time_window: (秒) 一個事件可以延續一條鏈的最大時間差
    :param min_length: (int) 我們關心的最小鏈長度
    :param chain_length: (int) 用於定義 segment 邊界的長度 (例如 10)
    :param npat: (int) 總 pattern 數 (用於邊界檢查，雖然此處用 // 即可)
    :return: (forward_lengths, reverse_lengths) 包含所有已完成鏈長度的列表
    """
    
    # active_chains 儲存 [ (t_last, p_last), length, direction ]
    active_chains = []
    
    completed_fwd = []
    completed_rev = []
    
    if not events:
        return [], []

    print(f"Analyzing {len(events)} total events (Rule: No cross-segment chains, CL={chain_length})...")
    
    for (t, p) in events:
        
        chains_to_add = []      # 儲存本輪要 "延續" 或 "種植" 的新鏈
        chains_to_remove = []   # 儲存本輪 "死亡" (pruned) 的舊鏈
        
        # --- 1. 修剪 (Prune) & 延續 (Extend) ---
        for chain in active_chains:
            (t_last, p_last), length, direction = chain
            
            # --- Prune Check (Time) ---
            # 檢查是否 "死亡" (時間過期)
            if t > t_last + time_window:
                chains_to_remove.append(chain)
                if length >= min_length:
                    if direction == 'fwd':
                        completed_fwd.append(length)
                    elif direction == 'rev':
                        completed_rev.append(length)
                continue # 鏈已死亡，不能再延續

            # --- *** 關鍵修改: Segment 檢查 *** ---
            # 檢查新的 p 和 p_last 是否在同一個 segment
            same_segment = (p // chain_length) == (p_last // chain_length)
            
            if not same_segment:
                # 跨越了 segment (例如 9 -> 10 或 10 -> 9)
                # 不允許延續。
                # 原始鏈 'chain' 保持 active 狀態，直到它自己 time out。
                continue

            # --- Extend Check (Forward) ---
            if p == p_last + 1 and (direction == 'fwd' or direction is None):
                # 複製並延續
                new_chain = [(t, p), length + 1, 'fwd']
                chains_to_add.append(new_chain)

            # --- Extend Check (Reverse) ---
            if p == p_last - 1 and (direction == 'rev' or direction is None):
                # 複製並延續
                new_chain = [(t, p), length + 1, 'rev']
                chains_to_add.append(new_chain)

        # --- 2. 種植 (Seed) ---
        # 每一個事件都是一條新鏈的 "種子" (V1 邏輯)
        chains_to_add.append( [(t, p), 1, None] )
        
        # --- 3. 更新 active_chains ---
        # (V1 邏輯)
        # 移除 "死亡" 的鏈
        for chain in chains_to_remove:
            if chain in active_chains: 
                active_chains.remove(chain)
        
        # 加入 "新" 的鏈 (延續的 + 種子)
        active_chains.extend(chains_to_add)

    # --- 4. 最終清理 ---
    # 遍歷結束後，處理所有還 "活著" 的鏈 (V1 邏輯)
    for (t_last, p_last), length, direction in active_chains:
        if length >= min_length:
            if direction == 'fwd':
                completed_fwd.append(length)
            elif direction == 'rev':
                completed_rev.append(length)
                
    return completed_fwd, completed_rev

def load_sorted_events(h5_file_path):
    """
    從 H5 檔案載入事件，並按時間排序，保留真實時序。
    (與 V1 相同)
    """
    print(f"Loading data from {h5_file_path}...")
    with h5py.File(h5_file_path, 'r') as h5f:
        if 'activations' not in h5f:
            print(f"Error: Dataset 'activations' not found in {h5_file_path}.", file=sys.stderr)
            return None, None
            
        all_data = h5f['activations'][:]
        act_times = all_data[0]
        pattern_ixs = all_data[2].astype(int)

    if act_times.size == 0:
        print("Warning: No activation events found.")
        return [], 0
    
    npat = np.max(pattern_ixs) + 1
    
    events = zip(act_times, pattern_ixs)
    
    sorted_events = sorted(events, key=lambda x: x[0])
    
    print(f"Loaded and sorted {len(sorted_events)} total events. Found {npat} patterns.")
    
    return sorted_events, npat

def plot_results(fwd_lengths, rev_lengths, npat, fig_name, min_length):
    """
    可視化所有找到的鏈的 "長度分佈"。
    (與 V1 相同 - 使用直方圖)
    """
    fig, ax = plt.subplots(figsize=(10, 6))
    
    fwd_count = len(fwd_lengths)
    rev_count = len(rev_lengths)
    
    if fwd_count == 0 and rev_count == 0:
        ax.text(0.5, 0.5, f'No chains (L >= {min_length}) found.', 
                 horizontalalignment='center', verticalalignment='center', 
                 transform=ax.transAxes, color='gray')
    else:
        # --- 繪製直方圖 ---
        max_len = 0
        if fwd_lengths: max_len = max(max_len, max(fwd_lengths))
        if rev_lengths: max_len = max(max_len, max(rev_lengths))
        
        # 設置 bins (例如：3, 4, ..., max_len)
        bins = np.arange(min_length - 0.5, max_len + 1.5, 1)

        ax.hist(fwd_lengths, bins=bins, alpha=0.7, label=f'Forward Chains (Total: {fwd_count})', color='blue')
        ax.hist(rev_lengths, bins=bins, alpha=0.7, label=f'Reverse Chains (Total: {rev_count})', color='red')
    
        ax.set_xlabel('Chain Length')
        ax.set_ylabel('Count')
        # 確保 x 軸刻度是整數
        ax.set_xticks(np.arange(min_length, max_len + 1, 1))
        ax.legend()

    ax.set_title(f'Distribution of Chain Lengths (Time Window={args.time_window}s, npat={npat})')
    
    fig.tight_layout()
    # 確保目錄存在
    os.makedirs('seg/plotting/fig_order/', exist_ok=True)
    save_path = f'seg/plotting/fig_order/{fig_name}.png'
    plt.savefig(save_path, dpi=300)
    plt.close()
    print(f"Saved plot to '{save_path}'")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description="Analyze replay chain lengths (segmented) based on time windows."
    )
    # --- 參數與原檔案相同 ---
    parser.add_argument('--namespace', type=str, default='lognormal')
    parser.add_argument('-p', '--patterns', type=int, default=1000)
    parser.add_argument('-m', '--mode', type=str, required=True)
    parser.add_argument('-s', '--system', type=str, required=True)
    
    # --- 混合演算法的參數 ---
    parser.add_argument(
        '-t', '--time_window', 
        type=float, 
        default=5,
        help="Time window (in seconds) to consider for chain continuation."
    )
    parser.add_argument(
        '-l', '--min_length', 
        type=int, 
        default=3,
        help="Minimum chain length to record. (您要求 3)"
    )
    parser.add_argument(
        '-cl', '--chain_length', 
        type=int, 
        default=10,
        help="The fixed length of each segment (e.g., 10) for boundary checks."
    )
    
    args = parser.parse_args()
    
    # 1. 載入並處理 recall 順序
    folder = data_path(args.namespace)
    h5_file = f"{folder}/{args.mode}_{args.system}_spontaneous{args.patterns}_activations.h5"
    sorted_events, npat = load_sorted_events(h5_file)

    if not sorted_events:
        sys.exit(1)
        
    if npat == 0:
        print("Error: npat is 0, cannot proceed.")
        sys.exit(1)
        
    if args.patterns != npat:
         print(f"Warning: Argument --patterns ({args.patterns}) does not match npat from file ({npat}). Using npat={npat}.")
         # npat (來自檔案) 將被用於分析

    # 3. 分析所有 "分段" 鏈 (使用新演算法)
    print("Analyzing for all segmented chains...")
    fwd_lengths, rev_lengths = find_segmented_chains(
        sorted_events, 
        args.time_window, 
        args.min_length,
        args.chain_length,
        npat
    )
    
    print("\n--- Analysis Results (Segmented Chains) ---")
    print(f"Total events analyzed: {len(sorted_events)}")
    print(f"Found {len(fwd_lengths)} forward chains (L >= {args.min_length})")
    print(f"Found {len(rev_lengths)} reverse chains (L >= {args.min_length})")
    
    if fwd_lengths:
        print(f"  Forward: Max={np.max(fwd_lengths)}, Mean={np.mean(fwd_lengths):.2f}")
    if rev_lengths:
        print(f"  Reverse: Max={np.max(rev_lengths)}, Mean={np.mean(rev_lengths):.2f}")
    
    # 4. 可視化 (使用 V1 圖表)
    fig_name = f'{args.mode}_{args.system}{npat}_chains_t{args.time_window}_L{args.min_length}_CL{args.chain_length}'
    plot_results(fwd_lengths, rev_lengths, npat, fig_name, args.min_length)