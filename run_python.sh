#!/bin/bash
#SBATCH -p short
#SBATCH -t 0:30:00
#SBATCH --mem=250G
#SBATCH -c 5
#SBATCH -C xeon
#SBATCH --open-mode=append
#SBATCH --job-name=hebb_activations

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32  # 雖然是 short job，但保持一致

#SBATCH --output=hebb_output_%j.log
#SBATCH --error=hebb_error_%j.log

echo "Starting job on $(hostname) at $(date)"

# 接收 $1=patterns, $2=system, $3=run

pixi run python analysis/fig1/weight_distributions.py 
pixi run python analysis/fig1/weight_distributions.py --system rate
pixi run python analysis/fig1/weight_distributions.py --system hebb_smooth_rate

echo "Job finished at $(date)"