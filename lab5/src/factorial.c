#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>

struct factl_args
{
    int start;
    int end;
    int mod;
    int thread_number;
};

void factorial(struct factl_args*);
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
int result = 1;
int resultmod = 1;

int main(int argc, char **argv) {
  int pnum = -1;
  int mod = -1;
  int k = -1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 'k'},
                                      {"pnum", optional_argument, 0, 0},
                                      {"mod", optional_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "k:", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'k':
          k = atoi(optarg);
          if (k <= 0)
          {
              printf("Invalid arguments (k)!\n");
              exit(EXIT_FAILURE);
          }
          break;
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            if (k <= 0)
            {
                printf("Invalid arguments (k)!\n");
                exit(EXIT_FAILURE);
            }
            break;
          case 1:
            pnum = atoi(optarg);
            if (pnum <= 0)
            {
                printf("Invalid arguments (pnum)!\n");
                exit(EXIT_FAILURE);
            }
            break;
          case 2:
            mod = atoi(optarg);
            if (mod <= 0)
            {
                printf("Invalid arguments (mod)!\n");
                exit(EXIT_FAILURE);
            }
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

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (k == -1 || pnum == -1 || mod == -1) {
    printf("Usage: %s -k \"num\" --pnum=\"num\" --mod=\"num\" \n",
           argv[0]);
    return 1;
  }
  
  
  pthread_t *threads = malloc(pnum * sizeof(pthread_t));
  struct factl_args* fun_args = malloc(pnum * sizeof(struct factl_args));
  int factorial_part = k / pnum;
  for (int i = 0; i < pnum; i++) {
      fun_args[i].start = (i*factorial_part) + 1;

      if (i != pnum - 1) {
        fun_args[i].end = (i + 1)*factorial_part;
      }
      else {
        fun_args[i].end = k;
      }
      
      fun_args[i].mod = mod;
      fun_args[i].thread_number = i + 1;
      
      if (pthread_create(&threads[i], NULL, (void *)factorial,
    			  (void *)&fun_args[i]) != 0) {
        perror("pthread_create");
        exit(1);
      }
  }
  
  for (int i = 0; i < pnum; i++) {
     if (pthread_join(threads[i], NULL) != 0) {
        perror("pthread_join");
        exit(1);
     }
  }

  printf("\nAll done, facterial %d = %d\n",k,result);
  printf(" %d! mod %d = %d\n", k, mod, resultmod );

  return 0;
}

void factorial(struct factl_args* args) {
  int start = args->start;
  int end = args->end;
  int mod = args->mod;
  int thread_number = args->thread_number;
  
  for (int i = start; i <= end; i++) {
    pthread_mutex_lock(&mut);

    result *= i;
    if(resultmod != 0)
    	resultmod = result % mod;
    printf("i: %d --- result fact = %d ---result mod %d = %d\n", i , result, mod, resultmod);
    
    pthread_mutex_unlock(&mut);
  }
}

