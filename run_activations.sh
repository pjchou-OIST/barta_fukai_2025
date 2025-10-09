#!/bin/bash
# 
# 這是給 Slurm 排程系統看的配置

#!/bin/bash
#SBATCH -p short
#SBATCH -t 0:30:00
#SBATCH --mem=250G
#SBATCH -c 5
#SBATCH -C xeon
#SBATCH --open-mode=append


# 配置 1：設定作業名稱
#SBATCH --job-name=hebb_activations


# 配置 5：指定輸出/錯誤日誌檔案
#SBATCH --output=hebb_output_%j.log
#SBATCH --error=hebb_error_%j.log

# ----------------------------------------------------
# 程式執行區域
# ----------------------------------------------------


echo "Starting job on $(hostname) at $(date)"

# 這是你原本的 Python 執行命令
pixi run python src/get_activations.py \
    --patterns 1000 \
    --system hebb \
    --run spontaneous

echo "Job finished at $(date)"