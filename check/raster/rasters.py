import numpy as np
import h5py
from tqdm import tqdm
import sys
import os
import matplotlib.pyplot as plt
import argparse  # 用於處理命令列參數

# --- 路徑設定 ---
current_dir = os.path.dirname(os.path.abspath(__file__)) 
src_dir = os.path.dirname(current_dir)                    
parent_dir = os.path.dirname(src_dir)                   
utils_path = os.path.join(parent_dir, 'src/')

sys.path.append(utils_path)

# 嘗試匯入專案模組
try:
    from utils import load_patterns, data_path
    from analysis import get_spike_counts
    from eigenvalues import get_W
except ImportError:
    print("警告: 找不到自定義模組 (utils, analysis, eigenvalues)。請確認路徑設定正確。")

def load_spikes(system, npat, start, end, namespace, target_file='spontaneous_short'):
    """
    載入 Spike 數據
    """
    folder = data_path(namespace)
    filename = f"{folder}/{system}_{target_file}{npat}.h5"

    try:
        with h5py.File(filename, "r") as h5f:
            # 讀取數據
            # 注意：這裡讀取的範圍可能需要根據實際檔案大小調整，這裡保留原本的 logic
            exc_indices, exc_times = h5f['spikes_exc'][:8000*4*int(end+1)].T
            inh_indices, inh_times = h5f['spikes_inh'][:2000*15*int(end+1)].T
    except FileNotFoundError:
        print(f"找不到檔案: {filename}")
        return np.array([]), np.array([])
    except Exception as e:
        print(f"讀取錯誤 {filename}: {e}")
        return np.array([]), np.array([])

    # 根據時間過濾
    mask_exc = (exc_times >= start) & (exc_times < end)
    spikes_exc = np.array(([exc_indices[mask_exc], exc_times[mask_exc]]))

    mask_inh = (inh_times >= start) & (inh_times < end)
    spikes_inh = np.array([inh_indices[mask_inh], inh_times[mask_inh]])

    return spikes_exc, spikes_inh

def plot_raster(spikes_exc, spikes_inh, system, fig_dir, t_range, max_exc=None, max_inh=None):
    """
    繪製 Raster Plot
    參數:
      t_range: tuple (start, end)
      max_exc: int or None, 只顯示前 N 個興奮性神經元
      max_inh: int or None, 只顯示前 N 個抑制性神經元
    """
    plt.figure(figsize=(12, 6)) # 調整長寬比適合 Raster

    # --- 1. 根據 max_exc/max_inh 篩選神經元 ---
    # spikes[0] 是 neuron index
    if max_exc is not None and spikes_exc.size > 0:
        mask = spikes_exc[0] < max_exc
        spikes_exc = spikes_exc[:, mask]
    
    if max_inh is not None and spikes_inh.size > 0:
        mask = spikes_inh[0] < max_inh
        spikes_inh = spikes_inh[:, mask]

    # --- 2. 計算 Y 軸偏移量 (Offset) ---
    # 讓抑制性神經元疊在興奮性神經元上方，避免重疊
    if max_exc is not None:
        # 如果有指定 max_exc (例如 800)，則 offset 就設為 800
        y_offset = max_exc
    elif spikes_exc.size > 0:
        # 如果沒指定，則自動偵測 Exc 的最大 index 並加 1
        y_offset = np.max(spikes_exc[0]) + 1
    else:
        y_offset = 0

    # --- 3. 繪圖 ---
    # 繪製 Excitatory (黑色)
    if spikes_exc.size > 0:
        plt.scatter(spikes_exc[1], spikes_exc[0], 
                    s=0.5, c='k', marker='|', alpha=0.6, label='Excitatory')
    
    # 繪製 Inhibitory (紅色，加上 offset)
    if spikes_inh.size > 0:
        plt.scatter(spikes_inh[1], spikes_inh[0] + y_offset, 
                    s=0.5, c='r', marker='|', alpha=0.6, label='Inhibitory')

    # --- 4. 美化與標籤 ---
    plt.title(f'Raster Plot: {system}')
    plt.xlabel('Time (s)')
    plt.xlim(t_range) # 設定 X 軸時間範圍

    # 設定 Y 軸標籤
    plt.ylabel('Neuron Index')
    
    # 畫一條虛線區隔兩者
    if y_offset > 0:
        plt.axhline(y=y_offset, color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
        # 在 Y 軸旁邊標註文字，類似論文風格
        plt.text(t_range[0], y_offset/2, 'Exc', va='center', ha='right', rotation='vertical', fontsize=10, color='gray')
        
        # 計算 inh 的高度位置標籤
        inh_top = (np.max(spikes_inh[0]) if spikes_inh.size > 0 else 0) + y_offset
        if max_inh: inh_top = max_inh + y_offset
        inh_center = y_offset + (inh_top - y_offset)/2
        plt.text(t_range[0], inh_center, 'Inh', va='center', ha='right', rotation='vertical', fontsize=10, color='gray')

    plt.legend(loc='upper right', markerscale=5)
    plt.tight_layout()
    
    # 儲存圖片
    plot_filename = f'{fig_dir}{system}_raster_t{t_range[0]}_{t_range[1]}_{max_exc}_{max_inh}.png'
    plt.savefig(plot_filename, dpi=500)
    plt.close()

if __name__ == '__main__':
    # --- 設定 Argument Parser ---
    parser = argparse.ArgumentParser(description='Generate Raster Plots from spike data.')
    
    # 時間範圍參數
    parser.add_argument('--start', type=float, default=50.0, help='Start time (seconds), default: 0')
    parser.add_argument('--end', type=float, default=70.0, help='End time (seconds), default: 20')
    
    # 神經元遮罩參數 (選填)
    parser.add_argument('--max_exc', type=int, default=None, 
                        help='Number of Excitatory neurons to plot (e.g., 800). Default: All')
    parser.add_argument('--max_inh', type=int, default=None, 
                        help='Number of Inhibitory neurons to plot (e.g., 200). Default: All')
    parser.add_argument('--save_csv', action='store_true', default=False,
                        help='Whether to save spike data as CSV files. Default: False')
    
    args = parser.parse_args()

    # --- 主程式邏輯 ---
    fig_dir = 'log_norm-STD_inhf1.1/check/rasters/'
    target_file = 'spontaneous_short_inhf1.1_'
    namespace = 'lognormal'
    
    # 您可以根據需要增減 systems 列表
    systems = ['hebb', 'sfa_hebb']
    
    print(f"Processing settings:")
    print(f"  Time Range: {args.start}s to {args.end}s")
    print(f"  Exc Neurons: {'All' if args.max_exc is None else args.max_exc}")
    print(f"  Inh Neurons: {'All' if args.max_inh is None else args.max_inh}")
    print(f"  Systems: {len(systems)}")
    
    for system in tqdm(systems):
        # 1. 載入數據
        spikes_exc, spikes_inh = load_spikes(system, 1000, args.start, args.end, namespace, target_file)

        # 確保輸出資料夾存在
        os.makedirs(f'{fig_dir}', exist_ok=True)

        # 3. 繪製並儲存 Raster Plot
        if spikes_exc.size > 0 or spikes_inh.size > 0:
            plot_raster(
                spikes_exc, 
                spikes_inh, 
                system, 
                fig_dir, 
                t_range=(args.start, args.end),
                max_exc=args.max_exc,
                max_inh=args.max_inh
            )