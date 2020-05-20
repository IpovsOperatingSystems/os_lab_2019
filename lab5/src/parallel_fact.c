#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static long int factorial_answer=1;

typedef struct{
    int *array_num;
    int begin;
    int end;
}PartOfArray;

void *Factorial(int *args){
    PartOfArray *local_part = (PartOfArray*)args;
  int local_answer = 1;
  for (int i = local_part->begin; i < local_part->end; i++)
    local_answer *= local_part->array_num[i];
  pthread_mutex_lock(&mutex);
  factorial_answer *= local_answer;
  pthread_mutex_unlock(&mutex);
}


int main(int argc, char **argv){
    
    int pnum=-1;
    int k=-1;
    int mod=-1;

      while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"pnum", required_argument, 0, 0},
                                      {"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            pnum = atoi(optarg);
              if (pnum <= 0) {
                printf("pnum must be positiv number%d\n", pnum);
                return 1;
            }       
            break;
          case 1:
            k = atoi(optarg);
            if (k <= 0) {
                printf("k must be positiv number%d\n", k);
                return 1;
            }
            break;
          case 2:
            mod = atoi(optarg);
            if (pnum <= 0) {
                printf("mod must be positiv number%d\n", mod);
                return 1;
            }
            break;
         
          default:
            printf("Index %d is out of options\n", option_index);
        }

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  

}

 if (pnum == -1 || k == -1 || mod == -1) {
    printf("Usage: %s --pnum \"num\" --k \"num\" --mod \"num\" \n",argv[0]);
    return 1;
  }

  int i;

    int *array = (int*)malloc(k * sizeof(int));
  for (i = 0; i < k; i++) {
    array[i] = i + 1;
  }

  int active_step = k > pnum ? (k / pnum) : 1;
  pthread_t threads[pnum];
  PartOfArray parts[pnum];   
  for(i = 0; i < pnum; i++) {
    parts[i].array_num = array;
    parts[i].begin = active_step * i;
    parts[i].end = (i + 1) * active_step;
  }
struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for(i = 0; i < pnum; i++) {
    if (pthread_create(&threads[i], NULL, Factorial, (void*)&parts[i])) {
      perror("\nERROR CREATE THREAD\n");
      return 1;
    }
  }

  for(i = 0; i < pnum; i++) {
    pthread_join(threads[i], NULL);
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  pthread_mutex_destroy(&mutex);
  free(array);
  printf("The factorial of %i equals %i.\n", k, factorial_answer);
  printf("The factorial of %i with module %i equals %i.\n", k, mod, factorial_answer % mod);
  printf("Elapsed time: %fms\n", elapsed_time);

  return 0;

}