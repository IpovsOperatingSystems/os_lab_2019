#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <getopt.h>

struct data
{
	uint64_t* f;
	uint64_t start, end;
	uint64_t mod;
};

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; //инициализация mutex
uint64_t shared;

void *Factorial(void *args) {
    struct data *d = (struct data *)args;
    for(uint64_t i = d->start; i < d->end; i++){
    	pthread_mutex_lock(&mtx); //перехват mutex
    	*(d->f) = (*(d->f)*i)%d->mod;
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
						printf("k must be more than zero\n");
					break;
				case 1:
					pnum = atoi(optarg);
					if(pnum < 1)
						printf("pnum must be more than zero\n");
					break;
				case 2:
					mod = atoi(optarg);
					if(mod < 1)
						printf("mod must be more than zero\n");
					break;
			}
			break;
		default:
			printf("Index %d: no such index\n", option_index);
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
	struct data d[pnum];
	for(uint64_t i = 0; i < pnum; i++)
	{
		d[i].f = &shared;
		d[i].start = i*k/pnum + 1;
		d[i].end = (i == (pnum - 1)) ? k + 1 : (i+1)*k/pnum + 1;
		d[i].mod = mod;
	}

    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    for (uint32_t i = 0; i < pnum; i++) {
        if (pthread_create(&threads[i], NULL, Factorial, (void *)&d[i])) {
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