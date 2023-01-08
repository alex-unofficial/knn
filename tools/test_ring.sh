#!/bin/bash
#SBATCH --partition=rome
#SBATCH --time=0:02:00
#SBATCH --job-name=test_ring
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=128

module load gcc/8.2.0 openmpi/4.1.2-6ojeowp gsl/2.5 pkgconf/1.7.3

make clean && make
srun ./bin/test_ring
