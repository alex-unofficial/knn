#!/bin/bash
#SBATCH --partition=rome
#SBATCH --time=1:00:00
#SBATCH --job-name=mpiknn
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=128
#SBATCH --mem=0

module load gcc/10.2.0 openmpi/4.0.5 gsl/2.5 pkgconf/1.7.3

make

OUTFILE=data/results/knn_$SLURM_JOB_ID.out
LOGFILE=data/results/knn.log

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
srun ./bin/mpiknn -i $1 -k $2 -o $OUTFILE -l $LOGFILE -m $3
