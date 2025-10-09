#!/bin/bash
# 
# 這是給 Slurm 排程系統看的配置

#!/bin/bash
#SBATCH -p compute
#SBATCH -t 48:00:00
#SBATCH --mem=250G
#SBATCH -c 5
#SBATCH -C xeon
#SBATCH --open-mode=append


# 配置 1：設定作業名稱
#SBATCH --job-name=hebb_simulation

#SBATCH --nodes=1                     # 請求 1 個節點
#SBATCH --ntasks=1                    # 在節點上啟動 1 個主任務
#SBATCH --cpus-per-task=32            # 為這個任務請求 32 個 CPU 核心


# 配置 5：指定輸出/錯誤日誌檔案
#SBATCH --output=hebb_output_%j.log
#SBATCH --error=hebb_error_%j.log

# ----------------------------------------------------
# 程式執行區域
# ----------------------------------------------------


echo "Starting job on $(hostname) at $(date)"

# 這是你原本的 Python 執行命令
pixi run python src/simulation.py \
  --system config/systems/hebb.yml \
  --run config/runtypes/perturbation.yml \
  --patterns 1000

echo "Job finished at $(date)"