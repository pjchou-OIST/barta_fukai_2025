#!/bin/bash
#SBATCH -p compute
#SBATCH -t 48:00:00
#SBATCH --mem=250G
#SBATCH -c 5
#SBATCH -C xeon
#SBATCH --open-mode=append
#SBATCH --job-name=hebb_simulation
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --output=hebb_stimuli100ms_output_%j.log
#SBATCH --error=hebb_stimuli100ms_error_%j.log

echo "Starting job on $(hostname) at $(date)"

# 接收 $1=system, $2=run, $3=patterns
echo "Running simulation with:"
echo "  System: $1"
echo "  Run:    $2"
echo "  Patterns: $3"
pixi run python src/simulation.py \
  --system $1 \
  --run $2 \
  --patterns $3

echo "Job finished at $(date)"