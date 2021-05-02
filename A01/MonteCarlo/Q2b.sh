#!/bin/bash
TIMEFORMAT=", time = %R"
gcc MonteCarlo.c -O0 -fopenmp -lm
for numDivisions in 50 100 500 1000 5000 10000 50000 100000 500000 1000000
do
  export OMP_NUM_THREADS=1
  for i in 1 2 3 4 5 6 7 8 9 10 11
  do
    sleep 0.1
    ./a.out 1 $numDivisions
  done
done
