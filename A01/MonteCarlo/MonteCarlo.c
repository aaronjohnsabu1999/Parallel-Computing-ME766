#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "omp.h"

#define f(x) cos(x)
#define pi   3.141592653589

int main(int argc, char *argv[])
{
  int numThreads = strtol(argv[1], (char **)NULL, 10), threadNum;
  long long int numSteps = strtol(argv[2], (char **)NULL, 10), total = 0, count = 0;
  float x, y, x0 = -pi/2.0, xn = pi/2.0, y0 = 0.0, yn = f(0.0);
  unsigned int seed = 0;
  srand(time(NULL));
  
  float time1 = omp_get_wtime();
  #pragma omp parallel private(x, y) reduction(+:total) reduction(+:count)
  for (long long int i = 0; i < (long long int)numSteps/numThreads; i++)
  {
    x = rand_r(&seed)*(xn - x0)/RAND_MAX + x0;
    y = rand_r(&seed)*(yn - y0)/RAND_MAX + y0;
    count += (f(x) > y);    
    total++;
  }
  float time2 = omp_get_wtime();
  float integral = (xn - x0) * (yn - y0) * count / total;
  printf("\nIntegral of cos(x) from %f to %f across %d total counts (numThreads = %d) is %f, time = %f", x0, xn, (int)total, numThreads, integral, time2-time1);
  
  return 0;
}
