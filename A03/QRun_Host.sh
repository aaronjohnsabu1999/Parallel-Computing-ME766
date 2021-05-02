#!/bin/bash
#SBATCH --nodes=1
#SBATCH --job-name=170070050A03
#SBATCH --partition=standard
#SBATCH --output=job.%J.out_node_40

module load compiler/intel/2018.2.199
cd ~/ME766/Assignments/A03/
gcc -o Q_Host Q_Host.c

TIMEFORMAT="%R"
for iter in 1 2 3 4 5 6 7 8 9 10
do
  for N in 120 240 480 720 960 1200 1500 1800 2100 2400 3600 4800 9600
  do
    sleep 0.1
    time ./Q_Host $N
  done
done
