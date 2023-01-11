#!/bin/bash
#SBATCH --partition=rome
#SBATCH --time=1:00:00
#SBATCH --job-name=mpiknn
#SBATCH --nodes=3
#SBATCH --ntasks-per-node=3
#SBATCH --cpus-per-task=10
#SBATCH --mem=5G

module load gcc/10.2.0 openmpi/4.0.5 gsl/2.5 pkgconf/1.7.3

make --quiet clean && make --quiet

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
srun ./bin/mpiknn $1 27
