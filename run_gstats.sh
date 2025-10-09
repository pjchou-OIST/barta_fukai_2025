#!/bin/bash
# 
# 這是給 Slurm 排程系統看的配置

#!/bin/bash
#SBATCH -p compute
#SBATCH -t 12:00:00
#SBATCH --mem=250G
#SBATCH -c 5
#SBATCH -C xeon
#SBATCH --open-mode=append

#SBATCH --nodes=1                     # 請求 1 個節點
#SBATCH --ntasks=1                    # 在節點上啟動 1 個主任務
#SBATCH --cpus-per-task=32            # 為這個任務請求 32 個 CPU 核心
#SBATCH --time=12:00:00               # 預計執行時間 8 小時


# 配置 1：設定作業名稱
#SBATCH --job-name=gstats_multiproc


# 配置 5：指定輸出/錯誤日誌檔案
#SBATCH --output=gstats_output_%j.log
#SBATCH --error=gstats_error_%j.log

# ----------------------------------------------------
# 程式執行區域
# ----------------------------------------------------


echo "Starting job on $(hostname) at $(date)"

# 這是你原本的 Python 執行命令
python src/gstats_multiproc.py \
        --name hebb_conductances \
        --patterns 1000

echo "Job finished at $(date)"