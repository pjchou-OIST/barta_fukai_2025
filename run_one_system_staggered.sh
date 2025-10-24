#!/bin/bash
#
# *** 這是你的新主腳本 (錯開提交版) ***
#
# 在登入節點 (login node) 執行:
#   bash ./run_one_system_staggered.sh
#
# 它會提交 *一個* 系統的所有 pattern 實驗，
# 每個工作流會被設定為 "延遲 1 小時" 開始
#

echo "========================================================"
echo "    開始提交 *錯開 1 小時* 的工作流..."
echo "========================================================"

# --- 1. 定義你的實驗參數 ---
# *** 在這裡修改你要跑的系統 ***
SYSTEM_TO_RUN="rate"

PATTERNS_LIST=(800 1200 1400 1600 1800 2000 2200 2400 2600 2800 3000)

# --- 2. 遍歷並提交 ---
# 用於計算延遲時間 (小時)
DELAY_HOURS=0

for PATS in "${PATTERNS_LIST[@]}"; do
    
    # 計算開始時間
    START_TIME="now+${DELAY_HOURS}hour"
    
    # 構建傳給 sbatch 的選項字串
    # 我們需要引號，這樣 " " 才會被當作一個單獨的 $3 參數傳給助手腳本
    SBATCH_OPTS="--begin=${START_TIME}"

    echo "------------------------------------------------"
    echo "提交: SYSTEM=${SYSTEM_TO_RUN}, PATTERNS=${PATS}"
    echo "設定開始時間: ${START_TIME}"

    # 執行助手腳本，並傳入 sbatch 選項
    # 注意: 這裡 $SBATCH_OPTS *需要* 引號
    bash ./workflow_iteration.sh "${SYSTEM_TO_RUN}" "${PATS}" "${SBATCH_OPTS}"
    
    # 增加下一個作業的延遲
    DELAY_HOURS=$((DELAY_HOURS + 1))
    
    # (可選) 在提交之間稍微暫停一下，避免太快
    sleep 1
done

echo "========================================================"
echo "          所有 11 個工作流已全部提交！"
echo "它們將會每隔一小時依序啟動。"
echo "你可以使用 'squeue -u $USER' 來監看進度。"
echo "========================================================"