#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <semaphore.h>

struct FactArgs {
    int begin;
    int end;
};

int factor = 1;
int modd = 0;
//pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;

void CountPart(void *args) {
    //pthread_mutex_lock(&mutx);
    sem_wait(&semaphore);
    struct FactArgs *arrs = (struct FactArgs *)args;
    int num = 1;
    for(int i = arrs->begin; i < arrs->end; i++)
    {
        num = num%modd * i%modd;
    }
    factor = factor % modd * num % modd;
    //pthread_mutex_unlock(&mutx);
    sem_post(&semaphore);
}


int main(int argc, char **argv)
{
    int k = 0;
    int pnum = 0;

    sem_init(&semaphore, 0, 1);//semaphore

    int opt = getopt(argc,argv, "k:");
    if (opt != 'k')
    {
        printf("Usage: %s -k \"num\" --mod \"num\" --pnum \"num\" \n", argv[0]);
        return 1;
    }
    else
    {
        k = atoi(optarg);
        if (k <= 0)
        {
            printf("k is a positive number\n");
            return 1;
        }
    }

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"mod", required_argument, 0, 0},
                                        {"pnum", required_argument, 0, 0},
                                        {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) 
        {
        case 0:
            switch (option_index) 
            {
            case 0:
                modd = atoi(optarg);
                if (modd <= 0) 
                {
                    printf("mod is a positive number\n");
                    return 1;
                }
                break;
            case 1:
                pnum = atoi(optarg);
                if (pnum <= 0) 
                {
                    printf("pnum is a positive number\n");
                    return 1;
                }
                if (pnum > k)
                {
                    printf("pnum must be less or equal to k\n");
                    pnum = k;
                    printf("pnum is equal to k now\n");
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

  if (optind < argc) 
  {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (modd == 0 || pnum == 0) 
  {
    printf("Usage: %s -k \"num\" --mod \"num\" --pnum \"num\" \n", argv[0]);
    return 1;
  }

  int ars = k/pnum;
  int left = k%pnum;
  struct FactArgs args[pnum];
  for(int i = 0; i < pnum; i++)
  {
      args[i].begin = ars*i + (left < i ? left : i) + 1;
      args[i].end = ars*(i+1) + (left < i+1 ? left : i+1) + 1;
  }

  pthread_t threads[pnum];
  for(int i = 0; i < pnum; i++)
  {
      if(pthread_create(&threads[i], NULL, (void*)CountPart, (void *)&(args[i])) != 0)//no problem
      {
          printf("pthread error\n");
          return 1;
      }
  }

  for(int i = 0; i < pnum; i++)
  {
      pthread_join(threads[i], NULL);
  }

sem_destroy(&semaphore);// delete

  factor %= modd;
  printf("Factorial %d by mod %d = %d\n", k, modd, factor);


    return 0;
}