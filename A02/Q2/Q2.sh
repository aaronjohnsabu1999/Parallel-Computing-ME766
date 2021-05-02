#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks-per-node=17
#SBATCH --time=06:00:00
#SBATCH --job-name=170070050A02Q02
#SBATCH --error=job.%J.err_node_40
#SBATCH --output=job.%J.out_node_40
#SBATCH --partition=standard

module load compiler/intel/2018.2.199
module unload gnu8/8.3.0

export I_MPI_FALLBACK=disable
mpicc -o Q2 Q2.c

for N in 120 240 480 720 960 1200 1500 1800 2100 2400 3600 4800 9600
do
  for numThreads in 2 3 4 5 6 7 9 11 13 16 17
  do
    sleep 0.1
    mpirun -np $numThreads ./Q2 $numThreads $N
  done
done
