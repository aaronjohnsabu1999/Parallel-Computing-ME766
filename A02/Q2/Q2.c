#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "mpi.h"

int main(int argc, char **argv)
{
  int numProcs = strtol(argv[1], (char **)NULL, 10), myProc, rootProc = numProcs - 1;
  int N = strtol(argv[2], (char **)NULL, 10), numRows = N / (numProcs - 1);
  int i, j, k, slave;
  float temp;
  float *A, *B, *C, *U, *slaveA, *slaveB, *slaveC, *slaveU;
  
  A = malloc(N*N* sizeof(float*));
  B = malloc(N*N* sizeof(float*));
  C = calloc(N*N, sizeof(float*));
  U = calloc(N*N, sizeof(float*));
  slaveA = malloc(numRows*N* sizeof(float*));
  slaveB = malloc(numRows*N* sizeof(float*));
  slaveC = calloc(numRows*N, sizeof(float*));
  slaveU = calloc(      N*N, sizeof(float*));
  
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myProc); 
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs); 
  
  double time2, time1 = MPI_Wtime();
  unsigned int seed = myProc;
  srand(myProc);
  
  if (myProc == rootProc)      // MASTER
  {
    // Random Initialization
    for (slave = 0; slave < numProcs - 1; slave++)
    {
      MPI_Recv(&slaveA[0], numRows*N, MPI_FLOAT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
      MPI_Recv(&slaveB[0], numRows*N, MPI_FLOAT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
      for (i = 0; i < numRows; i++)
        for (j = 0; j < N; j++)
        {
          A[((status.MPI_SOURCE)*numRows+i)*N + j] = slaveA[i*N + j];
          B[((status.MPI_SOURCE)*numRows+i)*N + j] = slaveB[i*N + j];
        }
    }
    
    // Matrix Multiplication
    for (slave = 0; slave < numProcs - 1; slave++)
    {
      MPI_Send(&A[slave*numRows], numRows*N, MPI_FLOAT, slave, 3, MPI_COMM_WORLD);
      MPI_Send(&B[0],                   N*N, MPI_FLOAT, slave, 4, MPI_COMM_WORLD);
    }
    
    for (slave = 0; slave < numProcs - 1; slave++)
    {
      MPI_Recv(&slaveC[0], numRows*N, MPI_FLOAT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
      for (i = 0; i < numRows; i++)
        for (j = 0; j < N; j++)
          C[((status.MPI_SOURCE)*numRows+i)*N + j] = slaveC[i*N + j];
    }
    
    for (i = 0; i < N; i++)
    {
      if (C[i*N + i] == 0)
      {
        for (j = i + 1; j < N; j++)
          if (j != 0)
            break;
        if (j == N)
          break;
        for (k = 0; k < N; k++)
        {
          temp       = C[i*N + k];
          C[i*N + k] = C[j*N + k];
          C[j*N + k] = temp;
        }
      }
    }
  }
  
  else                         // SLAVE
  {
    // Random Initialization
    for (i = 0; i < numRows; i++)
      for (j = 0; j < N; j++)
      {
        slaveA[i*N + j] = rand_r(&seed)/1000000.0;
        slaveB[i*N + j] = rand_r(&seed)/1000000.0;
      }
    MPI_Send(&slaveA[0], numRows*N, MPI_FLOAT, rootProc, 1, MPI_COMM_WORLD);
    MPI_Send(&slaveB[0], numRows*N, MPI_FLOAT, rootProc, 2, MPI_COMM_WORLD);
    
    // Matrix Multiplication
    MPI_Recv(&slaveA[0], numRows*N, MPI_FLOAT, rootProc, 3, MPI_COMM_WORLD, &status);
    MPI_Recv(&B[0],            N*N, MPI_FLOAT, rootProc, 4, MPI_COMM_WORLD, &status);
    for (i = 0; i < numRows; i++)
      for (j = 0; j < N; j++)
        for (k = 0; k < N; k++)
          slaveC[i*N + j] += slaveA[i*N + k] * B[k*N + j];
    MPI_Send(&slaveC[0], numRows*N, MPI_FLOAT, rootProc, 5, MPI_COMM_WORLD);
  }
  
  // Decomposition
  MPI_Bcast(C, N*N, MPI_FLOAT, rootProc, MPI_COMM_WORLD);
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      U[i*N + j] = C[i*N + j];

  for (i = 0; i < N; i++)
  {
    int numSplitRows = (N - i - 1) / (numProcs - 1);
    if (myProc == rootProc)    // MASTER
    {
      for (slave = 0; slave < numProcs - 1; slave++)
      {
        MPI_Recv(&slaveU[0], numSplitRows*N, MPI_FLOAT, slave, 6, MPI_COMM_WORLD, &status);
        for (j = 0; j < numSplitRows; j++)
          for (k = 0; k < N; k++)
            U[((i+1) + slave*numSplitRows + j)*N + k] = slaveU[j*N + k];
      }
    }
    else                       // SLAVE
    {
      for (j = (i+1) + myProc*numSplitRows; j < (i+1) + (myProc+1)*numSplitRows - 1; j++)
        for (k = 0; k < N; k++)
        {
          U[j*N + k] = U[j*N + k] - U[i*N + k]*(U[j*N + i]/U[i*N + i]);
          slaveU[(j - (i+1) - myProc*numSplitRows)*N + k] = U[j*N + k];
        }
      MPI_Send(&slaveU[0], numSplitRows*N, MPI_FLOAT, rootProc, 6, MPI_COMM_WORLD);
    }
  }

  time2 = MPI_Wtime() - time1;
  MPI_Finalize();

  if (myProc == rootProc)
    printf("Time taken for MPI implementation with (N = %d)\t and (numProcs = %d) = %.3f\n", N, numProcs, time2);

  return 0;
}