#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <getopt.h>
#include "Summa.h"
#include "utils.h"


void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  uint32_t threads_num = -1;
  uint32_t array_size = -1;
  uint32_t seed = -1;
        static struct option options[] = 	{{"seed", required_argument, 0, 0},
                                            {"array_size", required_argument, 0, 0},
                                            {"threads_num", required_argument, 0, 0},
                                            {0, 0, 0, 0}};

        int option_index = 0;
	while(1)
	{
	    int c = getopt_long(argc, argv, "f", options, &option_index);
		if (c == -1) break;
		switch(c)
		{
		case 0:
			switch(option_index)
			{
				case 0:
					seed = atoi(optarg);
					if(seed < 0)
						seed = -1;
					break;
				case 1:
					array_size = atoi(optarg);
					if(array_size < 1)
						array_size = -1;
					break;
				case 2:
					threads_num = atoi(optarg);
					if(threads_num < 1)
						threads_num = -1;
					break;
			}
			break;
		default:
			printf("Index %d is out of options\n", option_index);
		}
	}
        if (optind < argc) 
        {
                printf("Has at least one no option argument\n");
                return 1;
        }
        if (seed == -1 || array_size == -1 || threads_num == -1) 
        {
                printf("Usage: %s --seed \"num\" --array_size \"num\" --threads_num \"num\" \n", argv[0]);
                return 1;
        }
  
  pthread_t threads[threads_num];

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  struct SumArgs args[threads_num];
	for(int i = 0; i < threads_num; i++)
	{
		args[i].array = array;
		args[i].begin =     i*array_size/threads_num;
		args[i].end   = (i == (threads_num - 1)) ? array_size : (i+1)*array_size/threads_num;
	}
  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
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

  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n", elapsed_time);
  return 0;
}