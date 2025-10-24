#!/bin/bash
#SBATCH -p compute
#SBATCH -t 12:00:00
#SBATCH --mem=250G
#SBATCH -c 5
#SBATCH -C xeon
#SBATCH --open-mode=append
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --time=12:00:00
#SBATCH --job-name=gstats_multiproc
#SBATCH --output=gstats_output_%j.log
#SBATCH --error=gstats_error_%j.log

echo "Starting job on $(hostname) at $(date)"

# 接收 $1=name, $2=namespace, $3=patterns
echo "Running gstats with:"
echo "  Name: $1"
echo "  Namespace: $2"
echo "  Patterns: $3"
python src/gstats_multiproc.py \
  --name $1 \
  --namespace $2 \
  --patterns $3

echo "Job finished at $(date)"