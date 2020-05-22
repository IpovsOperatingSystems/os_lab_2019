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

bool is_timed_out = false;

void sig_handler(int signo) {
    is_timed_out = true;
    printf("The time is out!\n");
}

int kill_children(pid_t* pids, const size_t* size){
    for(size_t i = 0; i < size; ++i) {
        kill(pids[i], 9);
    }
    printf("All child processes were killed!");
    return 1;
}

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    int timeout = -1;
    bool with_files = false;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"seed",       required_argument, 0, 0},
                                          {"array_size", required_argument, 0, 0},
                                          {"pnum",       required_argument, 0, 0},
                                          {"timeout",    required_argument, 0, 0}, ///TODO change to optional
                                          {"by_files",   no_argument,       0, 'f'},
                                          {0, 0,                            0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        if (seed <= 0) {
                            printf("seed is a positive number\n");
                            return 1;
                        }

                        break;
                    case 1:
                        array_size = atoi(optarg);
                        if (array_size <= 0) {
                            printf("array_size is a positive number\n");
                            return 1;
                        }
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        if (pnum <= 0) {
                            printf("pnum is a positive number\n");
                            return 1;
                        }
                        break;

                    case 3:
                        timeout = atoi(optarg);
                        if (timeout <= 0) {
                            printf("timeout is a positive number\n");
                            return 1;
                        }
                        break;

                    case 4:
                        with_files = true;
                        break;

                    default:
                        printf("Index %d is out of options\n", option_index);
                        break;
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
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\ --timeout \"num\" \n",
               argv[0]);
        return 1;
    }

    int *array = (int *) malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);
    int active_child_processes = 0;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    int step = array_size / pnum;

    FILE *fl;
    int **pipefd;
    if (with_files) {
        fl = fopen("test.txt", "w");
    } else {
        pipefd = (int **) malloc(sizeof(int *) * pnum);
    }

    pid_t children[pnum];

    //send signal to ourself when the time is out
    signal(SIGALRM, sig_handler);
    alarm(timeout);

    for (int i = 0; i < pnum; i++) {
        if (!with_files) {
            pipefd[i] = (int *) malloc(sizeof(int) * 2);
            if (pipe(pipefd[i]) == -1) {
                printf("Pipe Failed");
                return 1;
            }
        }

        pid_t child_pid = fork();
        children[i] = child_pid;
        if (child_pid >= 0) {
            // successful fork
            active_child_processes += 1;
            if (child_pid == 0) { //CHILD
                printf("Pipe was created\n");

                struct MinMax min_max_i;
                // child process
                if (i != pnum - 1) {
                    min_max_i = GetMinMax(array, i * step, (i + 1) * step - 1);
                } else {
                    min_max_i = GetMinMax(array, i * step, array_size);
                }     // prallel somehow

                if (with_files) {
                    fwrite(&min_max_i.min, sizeof(int), 1, fl);
                    fwrite(&min_max_i.max, sizeof(int), 1, fl);
                } else {

                    write(pipefd[i][1], &min_max_i.min, sizeof(int));
                    write(pipefd[i][1], &min_max_i.max, sizeof(int));

                    close(pipefd[i][1]);
                }
                return 0;
            }
//PARENT

        } else {
            printf("Fork failed!\n");
            return 1;
        }
    }
    if (with_files) {
        fclose(fl);
        fl = fopen("test.txt", "r");
    }

    int status_pid;

    while (active_child_processes > 0) {
        if(is_timed_out)
            return kill_children(children, pnum);

        if (waitpid(-1, &status_pid, WNOHANG) > 0) { //If any child has died
            active_child_processes -= 1;
        }

    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    for (int i = 0; i < pnum; i++) {
        int min = INT_MAX;
        int max = INT_MIN;

        if (with_files) {     // read from files


            fread(&min, sizeof(int), 1, fl);
            fread(&max, sizeof(int), 1, fl);

        } else {
            //close(pipefd[i][1]);

            read(pipefd[i][0], &min, sizeof(int));
            read(pipefd[i][0], &max, sizeof(int));

            close(pipefd[i][0]);

            free(pipefd[i]);
        }


        if (min < min_max.min) min_max.min = min;
        if (max > min_max.max) min_max.max = max;
    }

    if (with_files) {
        fclose(fl);
    } else {
        free(pipefd);
    }

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