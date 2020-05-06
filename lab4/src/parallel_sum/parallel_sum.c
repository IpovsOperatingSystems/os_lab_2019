#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include <stdbool.h>

#include "utils.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  size_t i;

  for (i = args->begin; i < args->end; i++) {
    sum += args->array[i];
  }

  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  pthread_t threads[threads_num];

 while (true) {
    static struct option options[] = {{"threads_num", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"seed", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "?", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            threads_num = atoi(optarg);
             if (threads_num <= 0) {
                            printf("threads_num must be positiv number (better if bigger than 5)%d\n", threads_num);
                            return 1;
                        }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
                            printf("array_size must be positiv number (better if bigger than 5)%d\n", array_size);
                            return 1;
                        }
            break;
          case 2:
            seed = atoi(optarg);
             if (seed <= 0) {
                            printf("seed must be positiv number (better if bigger than 5)%d\n", seed);
                            return 1;
                        }
            break;
        }
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);     
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (threads_num == -1 || array_size == -1 || seed == -1) {
        printf("Usage: %s --threads_num \"num\" --array_size \"num\" --seed \"num\" \n",
           argv[0]);
    return 1;
  }


  int *array = malloc(sizeof(int) * array_size);

  struct SumArgs args[threads_num];

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

   int active_step = threads_num < array_size ? (array_size / threads_num) : 1;
   uint32_t i;
  for (i = 0; i < threads_num; i++) {
    if (i == threads_num - 1)
      args[i].end = array_size;

    args[i].array = array;
    args[i].begin = active_step * i;
    args[i].end = (i + 1) * active_step;
  }

  for (i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

    struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n\n", elapsed_time);
  return 0;
}
