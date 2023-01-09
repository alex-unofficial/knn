#!/bin/bash
#SBATCH --partition=rome
#SBATCH --time=1:00:00
#SBATCH --job-name=mpiknn
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=128

module load gcc/10.2.0 openmpi/4.0.5 gsl/2.5 pkgconf/1.7.3 julia/1.6.3

make --quiet clean && make --quiet

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
srun ./bin/mpiknn data/grid-1000000x3.dat 27 1000000 3
