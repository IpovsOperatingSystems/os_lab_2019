#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }

  int seed = atoi(argv[1]);
  if (seed <= 0) {
    printf("seed is a positive number\n");
    return 1;
  }

  int array_size = atoi(argv[2]);
  if (array_size <= 0) {
    printf("array_size is a positive number\n");
    return 1;
  }

  int *array = malloc(array_size * sizeof(int));
  GenerateArray(array, array_size, seed);

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  struct MinMax min_max = GetMinMax(array, 0, array_size);

   struct timeval finish_time;
   gettimeofday(&finish_time, NULL);

   double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
   elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
  
  /*int i;
  for(i=0;i<array_size;i++){
      printf("%d\n",array[i]);
  }
  free(array);*/

  printf("min: %d\n", min_max.min);
  printf("max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);

  return 0;
}
