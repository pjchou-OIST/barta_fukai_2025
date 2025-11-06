#!/bin/bash
#SBATCH -p compute
#SBATCH -t 72:00:00
#SBATCH --mem=250G
#SBATCH -C xeon
#SBATCH --open-mode=append
#SBATCH --job-name=hebb_simulation
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=10

echo "Starting job on $(hostname) at $(date)"

# 接收 $1=system, $2=run, $3=patterns
echo "Running parallel simulation with:"
echo "  System: $1"
echo "  Run:    $2"
echo "  Patterns: $3"
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
pixi run python src/parallel_simulation.py \
  --system $1 \
  --run $2 \
  --patterns $3

echo "Job finished at $(date)"