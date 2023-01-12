#!/bin/bash
#SBATCH --partition=rome
#SBATCH --time=1:00:00
#SBATCH --job-name=test_sedm
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=128

module load gcc/10.2.0 openmpi/4.0.5 gsl/2.5 pkgconf/1.7.3

make --quiet bin/test_sedm

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./bin/test_sedm $1 $2 $3
