#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed < 0) { 
              printf("Seed: %d cannot be negative\n", seed); 
              return -1; 
            }
            break;
          case 1:
            array_size = atoi(optarg);
             if (array_size < 0) { 
              printf("Array size: %d cannot be negative\n", array_size); 
              return -1; 
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum < 0) { 
              printf("Pnum: %d cannot be negative\n", pnum);
              return -1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  FILE *f_min_max;
  int my_pipe[2];
  if (pipe(my_pipe)) {
    printf("Error with pipe");
    return 1;
  }
  if(with_files) f_min_max = fopen("/workspaces/os_lab_2019/lab3/src/min_max_file.txt", "w");
  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        // parallel somehow
        struct MinMax min_max = GetMinMax(array, i*array_size/pnum, ((i+1)*array_size)/pnum);
        if (with_files) {
          // use files here
          fwrite(&min_max.min, sizeof(int), 1, f_min_max);
          fwrite(&min_max.max, sizeof(int), 1, f_min_max);
          fclose(f_min_max); //закрываем файл для child потока
        } 
        else {
          close(my_pipe[0]);
          write(my_pipe[1], &min_max.min, sizeof(int));
          write(my_pipe[1], &min_max.max, sizeof(int));
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  
  if(with_files){
     fclose(f_min_max);
     f_min_max = fopen("/workspaces/os_lab_2019/lab3/src/min_max_file.txt", "r");
  }

  while (active_child_processes > 0) {
    // your code here
    wait(NULL);
    active_child_processes -= 1;
  }


  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      fread(&min, sizeof(int), 1, f_min_max);
      fread(&max, sizeof(int), 1, f_min_max);
    } else {
      close(my_pipe[1]);
      read(my_pipe[0], &min, sizeof(int));
      read(my_pipe[0], &max, sizeof(int));
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  if(with_files) fclose(f_min_max);
  close(my_pipe[0]);

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  
  return 0;
}