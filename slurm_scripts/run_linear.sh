#!/bin/bash
#SBATCH -p compute
#SBATCH --mem=250G
#SBATCH -c 5
#SBATCH -C xeon
#SBATCH --open-mode=append
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --time=24:00:00
#SBATCH --job-name=linear_sensitivity
#SBATCH --output=linear_output_%j.log
#SBATCH --error=linear_error_%j.log

echo "Starting job on $(hostname) at $(date)"

# 接收 $1=system, $2=npat
echo "Running linear sensitivity with:"
echo "  System: $1"
echo "  NPat: $2"
python src/linear_sensitivity.py \
    --system $1 \
    --npat $2

echo "Job finished at $(date)"