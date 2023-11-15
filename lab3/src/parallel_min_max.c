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

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    bool with_files = false;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
                {"seed", required_argument, 0, 0},
                {"array_size", required_argument, 0, 0},
                {"pnum", required_argument, 0, 0},
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
                            exit(0);
                        }
                        break;
                    case 1:
                        array_size = atoi(optarg);
                        if (array_size < 1) {
                            exit(0);
                        }
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        if (pnum < 1) {
                            exit(0);
                        }
                        break;
                    case 3:
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

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    FILE* fp;
    char* filename = "min_max.txt";
    int fd[pnum][2];

    int active_child_processes = 0;
    int part = (float)array_size / pnum;

    if (with_files) {
        if (rollback_file(&fp, filename) == 1) { return 1;}
        if (open_file(&fp, filename, "a+") == 1) { exit(1); }
    }
    /*else if (pipe(fd) < 0) {
        printf("Pipes failed!\n");
        return 1;
    }*/

    for (int i = 0; i < pnum; i++) {
        if (!with_files) {
            if (pipe(fd[i])<0)
            {
                printf("Error while making pipe");
                return 1;
            }
        }
        pid_t child_pid = fork();
        if (child_pid < 0) {
            printf("Fork failed!\n");
            return 1;
        }
        else {
            active_child_processes++;
            if (child_pid == 0) {
                int arr_start = part * (float)i;
                int arr_end = arr_start + part;
                struct MinMax fork_min_max = GetMinMax(array, arr_start, arr_end);

                if (with_files)
                {
                    fprintf(fp, "%d %d\n", fork_min_max.min, fork_min_max.max);
                } else
                {
                    write(fd[i][1], &fork_min_max.min, sizeof(int));
                    write(fd[i][1], &fork_min_max.max, sizeof(int));
                    close(fd[i][1]);
                }
                exit(0);
            }
        }
    }
    while (active_child_processes > 0) {
        wait(0);
        active_child_processes--;
    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    if (with_files) {
        fclose(fp);
        open_file(&fp, filename, "r");
    }

    for (int i = 0; i < pnum; i++) {
        int min = INT_MAX;
        int max = INT_MIN;
        if (with_files)
        {
           fscanf(fp, "%d %d\n", &min, &max);
        }
        else
        {
            read(fd[i][0], &min, sizeof(int));
            read(fd[i][0], &max, sizeof(int));
            close(fd[i][0]);
        }

        if (min < min_max.min) min_max.min = min;
        if (max > min_max.max) min_max.max = max;
    }
    if (with_files) fclose(fp);

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