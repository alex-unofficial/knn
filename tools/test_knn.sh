#!/bin/bash
#SBATCH --partition=rome
#SBATCH --time=1:00:00
#SBATCH --job-name=test_knn
#SBATCH --nodes=1
#SBATCH --ntasks=128

module load gcc/8.2.0 openmpi/4.1.2-6ojeowp gsl/2.5 pkgconf/1.7.3

make clean && make ./bin/test_knn

export OMP_NUM_THREADS=$SLURM_NTASKS
./bin/test_knn
