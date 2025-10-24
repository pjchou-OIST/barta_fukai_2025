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

#SBATCH --job-name=genconn
#SBATCH --output=genconn_output_%j.log
#SBATCH --error=genconn_error_%j.log

echo "Starting job on $(hostname) at $(date)"

# 接收 $1 作為 patterns 參數
echo "Running with patterns: $1"
python src/genconn.py --patterns $1

echo "Job finished at $(date)"