#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
 
#include <getopt.h>

 

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  int i;
    //printf("%d %d ",args->begin,args->end);

    for (i=args->begin;i<args->end;i++){
        //printf("%d",i);
        sum+=*(args->array+i);
    }
  // TODO: your code here 
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  int pnum = -1;
  bool with_files = false;
 uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  pthread_t threads[threads_num];
  while (true) {
    static struct option options[] = {{"threads_num", required_argument, 0, 0},
                                      {"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {0, 0, 0, 0}};
 
    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    //printf("%d",c);
    if (c == -1) break;
    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            threads_num = atoi(optarg);
            //printf("%d", threads_num);
            break;
          case 1:
            seed = atoi(optarg);
            break;
          case 2:
            array_size = atoi(optarg);
            
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case '?':
        break;
 
      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }


  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  



  /*
   * TODO:
   * your code here
   * Generate array here
   */


  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array,  array_size,  seed);
  int i;


  struct SumArgs args[threads_num];
  //struct SumArgs buff;
  for (i=0;i<threads_num;i++){
      //args[i]=buff;
      args[i].begin=array_size/threads_num*i;
      args[i].end=array_size/threads_num*(i+1);
      args[i].array=array;

  }
  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
    printf("Elapsed time: %fms\n", elapsed_time);
  free(array);
  printf("Total: %d\n", total_sum);
  return 0;
}
