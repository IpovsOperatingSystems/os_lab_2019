#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <getopt.h>

struct somedata
{
	uint64_t* a;
	uint64_t s, e;
	uint64_t m;
};

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; //инициализация mutex
uint64_t shared;

void *ThreadSum(void *args) {
    struct somedata *smd = (struct somedata *)args;
    for(uint64_t i = smd->s; i < smd->e; i++){
    	pthread_mutex_lock(&mtx); //перехват mutex
    	*(smd->a) = (*(smd->a)*i)%smd->m;
    	pthread_mutex_unlock(&mtx); //освобождение mutex
    }
    return 0;
}

int main(int argc, char **argv) {
    shared = 1;
    uint32_t k = -1;
    uint32_t pnum = -1;
    uint32_t mod = -1;
    static struct option options[] = {{"k", required_argument, 0, 0},
                                    {"pnum", required_argument, 0, 0},
                                    {"mod", required_argument, 0, 0},
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
					k = atoi(optarg);
					if(k < 0)
						printf("k is a positive number!\n");
					break;
				case 1:
					pnum = atoi(optarg);
					if(pnum < 1)
						printf("pnum is a positive number!\n");
					break;
				case 2:
					mod = atoi(optarg);
					if(mod < 1)
						printf("mod is a positive number!\n");
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
        if (k == -1 || pnum == -1 || mod == -1) 
        {
                printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n", argv[0]);
                return 1;
        }
    pthread_t threads[pnum]; //создаем потоки
	struct somedata smd[pnum];
	for(uint64_t i = 0; i < pnum; i++)
	{
		smd[i].a = &shared;
		smd[i].s = i*k/pnum + 1;
		smd[i].e = (i == (pnum - 1)) ? k + 1 : (i+1)*k/pnum + 1;
		smd[i].m = mod;
	}

    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    for (uint32_t i = 0; i < pnum; i++) {
        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&smd[i])) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }
    
    for (uint32_t i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }
    
    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);
    
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
    
    
    printf("Total: %d\n", shared);
    printf("Elapsed time: %fms\n", elapsed_time);
    return 0;
}
