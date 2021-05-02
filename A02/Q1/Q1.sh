#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks-per-node=16
#SBATCH --time=05:59:59
#SBATCH --job-name=170070050A02Q01
#SBATCH --error=job.%J.err_node_40
#SBATCH --output=job.%J.out_node_40
#SBATCH --partition=standard

module load compiler/gcc/8.3.0
gcc -o Q1 Q1.c -fopenmp

for N in 120 240 480 720 960 1200 1500 1800 2100 2400 3600 4800 9600
do
  for numThreads in 1 2 3 4 5 6 8 10 12 15 16
  do
    export OMP_NUM_THREADS=$numThreads
    sleep 0.1
    ./Q1 $numThreads $N
  done
done