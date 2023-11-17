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
#include <signal.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int open_file(FILE** fp, char* filename, char* flag) {
    *fp = fopen(filename, flag);
    if (*fp == NULL) {
        printf("File %s trying open with failure", filename);
        return 1;
    } 
    return 0;
}

int rollback_file(FILE** fp, char* filename) {
    if (open_file(fp, filename, "w") == 1) { return 1; }
    fclose(*fp);
    return 0;
}

pid_t* child_pids; 
int child_num; 

static void PSIGKILL(int signo) {
    printf("SIGKILLing\n");
    printf("child num - %d\n", child_num);
    for (int i=0; i < child_num; i++) {
        printf("%d\t", i);
        kill(child_pids[i], SIGKILL);
    }
}


int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    int timeout = -1;
    bool with_files = false;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"timeout", required_argument, 0, 0},
            {"by_files", no_argument, 0, 'f'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        if (seed < 0) {
                            printf("seed must be positiv number: %d\n", seed);
                            return 1;
                        }
                        break;
          	        case 1:
            	        array_size = atoi(optarg);
                        if (array_size < 1) {
                            printf("array_size must be positiv number: %d\n", array_size);
                            return 1;
                        }
                        break;
         	        case 2:
           	            pnum = atoi(optarg);
                        if (pnum < 1) {
                            printf("pnum must be positiv number: %d\n", pnum);
                            return 1;
                        }
                        break;
           	        
                    case 3:
                        timeout = atoi(optarg);
                        if (timeout < 1) {
                            printf("timeout must be positiv number: %d\n", timeout);
                            return 1;
                        }
                    case 4:
            	        with_files = true;
              	        break;

          	    defalut:
                  printf("Index %d is out of options\n", option_index);
                } break;
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

    FILE* fp;
    char* filename = "min_max.txt";
    int pipefd[2];

    if (with_files) {
        if (rollback_file(&fp, filename) == 1) { return 1;}
        if (open_file(&fp, filename, "a+") == 1) { exit(1); }
    } 
    else if (pipe(pipefd) < 0) {
        printf("Pipes failed!\n");
        return 1;
    }

    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    if (timeout != -1) {
        printf("\nSet allarm on %d sec\n", timeout);
        alarm(timeout);
        signal(SIGALRM, PSIGKILL);
    }

    int active_child_processes = 0;
    int part = (float)array_size / pnum;

    for (int i = 0; i < pnum; i++) {
        pid_t child_pid = fork();
        if (child_pid < 0) return 1;

        else {
            
            active_child_processes++;
            child_num++;
            child_pids = (pid_t*)realloc(child_pids, sizeof(pid_t) * child_num);
            child_pids[child_num - 1] = child_pid;
            
            if (child_pid == 0) {
                int arr_start = part * (float)i;
                int arr_end = arr_start + part;
	        	struct MinMax fork_min_max = GetMinMax(array, arr_start, arr_end);

                if (with_files) 
                {
                    fprintf(fp, "%d %d\n", fork_min_max.min, fork_min_max.max);                                  
	        	} else 
                {
                    int w1 = write(pipefd[1], &fork_min_max.min, sizeof(int));
                    int w2 = write(pipefd[1], &fork_min_max.max, sizeof(int));
	        	} 
                exit(0);
	    	}
        }
    }
    while (active_child_processes > 0) {
        wait(NULL);
    	active_child_processes--;
    }

    
    if (with_files) { 
        fclose(fp);
        open_file(&fp, filename, "r"); 
    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    for (int i = 0; i < pnum; i++) {
        int min = INT_MAX;
        int max = INT_MIN;
        if (with_files) 
        { 
            int result = fscanf(fp, "%d %d\n", &min, &max); 
        }
        else 
        {
            int r1 = read(pipefd[0], &min, sizeof(int));
            int r2 = read(pipefd[0], &max, sizeof(int));
        }

    	if (min < min_max.min) min_max.min = min;
    	if (max > min_max.max) min_max.max = max;
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    if (with_files) fclose(fp);
    free(array);

    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    fflush(NULL);
    return 0;
}
