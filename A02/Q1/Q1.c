#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "omp.h"

int main(int argc, char *argv[])
{
  int numThreads = strtol(argv[1], (char **)NULL, 10), N = strtol(argv[2], (char **)NULL, 10);
  int i, j, k;
  float temp;
  float *A, *B, *C, *U;
  
  A = malloc(N*N*sizeof(float*));
  B = malloc(N*N*sizeof(float*));
  C = malloc(N*N*sizeof(float*));
  U = malloc(N*N*sizeof(float*));
  
  unsigned int seed = 0;
  srand(time(NULL));

  float time1 = omp_get_wtime();
  
  // Random Initialization
  #pragma omp parallel for private(j)
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
    {
      A[i*N + j] = rand_r(&seed)/1000000.0;
      B[i*N + j] = rand_r(&seed)/1000000.0;
    }
  
  // Matrix Multiplication
  #pragma omp parallel for private(j, k)
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
    {
      C[i*N + j] = 0;
      for (k = 0; k < N; k++)
        C[i*N + j] += A[i*N + k] * B[k*N + j];
      U[i*N + j] = C[i*N + j];
    }

  // Decomposition
  for (i = 0; i < N; i++)
  {
    if (C[i*N + i] == 0)
    {
      for (j = i + 1; j < N; j++)
        if (j != 0)
          break;
      if (j == N)
      {
        printf("\nCannot be decomposed");
        break;
      }
      for (k = 0; k < N; k++)
      {
        temp       = C[i*N + k];
        C[i*N + k] = C[j*N + k];
        C[j*N + k] = temp;
      }
    }
    for (j = i + 1; j < N; j++)
      #pragma omp parallel for private(k)
      for (k = 0; k < N; k++)
        U[j*N + k]  = U[j*N + k] - U[i*N + k]*(U[j*N + i]/U[i*N + i]);
  }

  float time2 = omp_get_wtime();
  
  // Print Commands
  printf("Time taken for OpenMP implementation with (N = \t%d) and (numThreads = %d) = %.3f\n", N, numThreads, time2-time1);
  /*
  printf("\nA : \n");
  for(i = 0; i < N; i++)
  {
    for(j = 0; j < N; j++)
      printf("%15.3f\t", A[i*N+j]);
    printf("\n");
  }
  printf("\nB : \n");
  for(i = 0; i < N; i++)
  {
    for(j = 0; j < N; j++)
      printf("%15.3f\t", B[i*N+j]);
    printf("\n");
  }
  printf("\nC : \n");
  for(i = 0; i < N; i++)
  {
    for(j = 0; j < N; j++)
      printf("%15.3f\t", C[i*N+j]);
    printf("\n");
  }
  printf("\nU : \n");
  for(i = 0; i < N; i++)
  {
    for(j = 0; j < N; j++)
      printf("%15.3f\t", U[i*N+j]);
    printf("\n");
  }
  */
  
  return 0;
}