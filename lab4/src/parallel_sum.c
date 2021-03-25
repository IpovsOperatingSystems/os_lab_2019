#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>

#include "task1/utils.h"


struct SumArgs {
    int* array;
    int begin;
    int end;
};


int Sum(const struct SumArgs* args) {
    int sum = 0;
    for (size_t i = args->begin; i < args->end; ++i) {
        sum += args->array[i];
    }
    return sum;
}


void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs*)args;
    return (void *)(size_t)Sum(sum_args);
}



int main(int argc, char **argv) {
    uint32_t threads_num = -1;
    uint32_t array_size = -1;
    uint32_t seed = -1;
  
    while (1) {
        int currennt_optind = optind ? optind : 1;
        static struct option options[] = {
            {"threads_num", required_argument, 0, 0},
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        int c = getopt_long(argc, argv, "?", options, &option_index);
        if (c==-1) break;

        switch (c) {
            case 0:
                switch (option_index) 
                {
                case 0:
                    threads_num = atoi(optarg);
                    if (threads_num < 1) {
                        printf("threads_num must be positive number: %d\n", threads_num);
                        return 1;
                    }
                    break;
                case 1:
                    seed = atoi(optarg);
                    if (seed < 1) {
                        printf("seed must be positiv number: %d\n", seed);
                        return 1;
                    }
                    break;
                case 2:
                    array_size = atoi(optarg);
                    if (array_size < 1) {
                        printf("array_size must be positiv number: %d\n", array_size);
                        return 1;
                    }
                    break;
                default:
                    printf("Index %d is out of options\n", option_index);
                    break;
                }
            case '?':
                break;
            default:
                printf("getopt returned character code0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has no one no option argument\n");
        return 1;
    }

    if (threads_num == -1 || seed == -1 || array_size == -1) {
        printf("Usage: %s --threads_num \"num\' --seed \"num\" --array_size \"num\"\n", argv[0]);
            return 1;
    }
    
    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);
    
    struct timeval t_start;
    gettimeofday(&t_start, NULL);
    
    pthread_t threads[threads_num];
    struct SumArgs args[threads_num];
    int step = array_size / threads_num;

    for (uint32_t i = 0; i < threads_num; ++i) {
        args[i].array = array;
        args[i].begin = i * step;
        if (i < threads_num - 1) {
            args[i].end = (i + 1) * step;
        } else {
            args[i].end = array_size;
        }
    }

    for (uint32_t i = 0; i < threads_num; i++) {
        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }

    int total_sum = 0;
    for (uint32_t i = 0; i < threads_num; i++) {
        int current_sum = 0;
        pthread_join(threads[i], (void*)&current_sum);
        total_sum += current_sum;
    }

    struct timeval t_end;
    gettimeofday(&t_end, NULL);

    double elapsed_time = 1000.0 * (t_end.tv_sec - t_start.tv_sec);
    elapsed_time += (t_end.tv_usec - t_start.tv_usec) / 1000.0;
    free(array);
    printf("Total: %d\n", total_sum);
    printf("Elapsed time: %f ms\n\n", elapsed_time);
    return 0;
}
