#include<stdio.h>
#include<stdlib.h>

// Matrix Multiplication
__global__ void matrixMul(float *A, float *B, float *C, int N)
{
  int i = threadIdx.y + blockIdx.y * blockDim.y;
  int j = threadIdx.x + blockIdx.x * blockDim.x;
  int k;
  
  if (i < N && j < N)
  {
    float temp = 0;
    for (k = 0; k < N; k++)
      temp += A[i*N + k] * B[k*N + j];
    C[i*N + k] = temp;
  }
}

int main(int argc, char *argv[])
{
  int N = strtol(argv[1], (char **)NULL, 10);
  size_t size = N*N*sizeof(float);
  float *A, *B, *C;
  
  cudaMallocManaged(&A, size);
  cudaMallocManaged(&B, size);
  cudaMallocManaged(&C, size);
  
  // Random Initialization
  for (int i = 0; i < N*N; i++)
  {
    A[i] = rand() / 1000000.0;
    B[i] = rand() / 1000000.0;
  }
  
  // Grid Dimensions
  int threads = 10 * 10;
  int blocks  = (N + threads - 1) / threads;
  // Kernel Launch Parameters
  dim3 THREADS (threads, threads);
  dim3 BLOCKS  ( blocks,  blocks);
  // Launch Kernel
  matrixMul<<<BLOCKS, THREADS>>>(A, B, C, N);
  cudaDeviceSynchronize();
  
  // Print Command
  printf("Time taken for CUDA implementation with (N = \t%d) = ", N);
  
  return 0;
}

  