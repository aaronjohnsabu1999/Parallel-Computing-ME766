#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
  int N = strtol(argv[1], (char **)NULL, 10), i, j, k;
  size_t size = N*N*sizeof(float*);
  float *A, *B, *C, temp;
  
  A = malloc(size);
  B = malloc(size);
  C = malloc(size);
  
  // Random Initialization
  for (i = 0; i < N*N; i++)
  {
    A[i] = rand() / 1000000.0;
    B[i] = rand() / 1000000.0;
  }
  
  // Matrix Multiplication
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
    {
      temp = 0;
      for (k = 0; k < N; k++)
        temp += A[i*N + k] * B[k*N + j];
      C[i*N + k] = temp;
    }
  
  // Print Command
  printf("Time taken for HOST implementation with (N = \t%d) = ", N);
  
  return 0;
}