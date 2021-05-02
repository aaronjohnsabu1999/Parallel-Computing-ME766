#!/bin/bash

nvcc -o Q_CUDA Q_CUDA.cu -ccbin "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\Hostx64\x64"

TIMEFORMAT="%R"
for iter in 1 2 3 4 5 6 7 8 9 10
do
  for N in 120 240 480 720 960 1200 1500 1800 2100 2400 3600 4800 9600
  do
    sleep 0.1
    time ./Q_CUDA $N
  done
done
