#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "omp.h"

#define f(x) cos(x)
#define pi   3.141592653589

int main(int argc, char *argv[])
{
  int numThreads = strtol(argv[1], (char **)NULL, 10);
  long long int i, numSteps = strtol(argv[2], (char **)NULL, 10);
  float x0 = -pi/2.0, xn = pi/2.0, x, stepSize, integral = 0.0;
  stepSize = (xn - x0) / numSteps;
  numSteps = (xn - x0) / stepSize;
  
  float time1 = omp_get_wtime();
  #pragma omp parallel for private(x) reduction(+:integral)
  for(i = 0; i <= numSteps; i++)
  {
    x = x0 + i * stepSize;
    integral += f(x);
  }
  float time2 = omp_get_wtime();
  integral *= stepSize;
  printf("\nIntegral of cos(x) from %f to %f across %d total counts (numThreads = %d) is %f, time = %f", x0, xn, (int)numSteps, numThreads, integral, time2-time1);
  
  return 0;
}