#!/bin/bash
TIMEFORMAT=", time = %R"
gcc Trapezoidal.c -O0 -fopenmp -lm
for numThreads in 1 2 3 4 5 6 7 8
do
  export OMP_NUM_THREADS=$numThreads
  for i in 1 2 3 4 5 6 7 8 9 10 11
  do
    sleep 0.1
    ./a.out $numThreads 10000000
  done
done
