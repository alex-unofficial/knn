#!/bin/bash
#SBATCH --partition=rome
#SBATCH --time=1:00:00
#SBATCH --job-name=test_knn
#SBATCH --nodes=1
#SBATCH --ntasks=128

module load gcc/10.2.0 openmpi/4.0.5 gsl/2.5 pkgconf/1.7.3 julia/1.6.3

make clean && make ./bin/test_knn

export OMP_NUM_THREADS=$SLURM_NTASKS
./bin/test_knn
