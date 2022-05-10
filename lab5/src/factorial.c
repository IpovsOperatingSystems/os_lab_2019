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

#include <errno.h>
#include <pthread.h>

struct Segment
{
    int begin;
    int end;
    int mod;
};

int result = 1;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void factorial(void* segments)
{

    struct Segment* segment = (struct Segment*)segments;
    int temp_result = 1;
    for (int i = (*segment).begin; i <= (*segment).end; i++)
    {
        temp_result *= ((i % (*segment).mod) == 0 ? 1 : i);
        sleep(1);

    }
    pthread_mutex_lock(&mut);
    result *= temp_result;
    pthread_mutex_unlock(&mut);
}

int main(int argc, char** argv)
{
    int k = -1;
    int pnum = -1;
    int mod = -1;

    while (true)
    {
        int current_optind = optind ? optind : 1;

        static struct option options[] = { { "k", required_argument, 0, 0},
                                          { "pnum", required_argument, 0, 0},
                                          { "mod", required_argument, 0, 0},
                                          { 0, 0, 0, 0} };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c)
        {
        case 0:
            switch (option_index)
            {
            case 0:
                k = atoi(optarg);
                if (k <= 0)
                {
                    printf("k is a positive number\n\n");
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
                break;
            case 2:
                mod = atoi(optarg);
                if (mod <= 0)
                {
                    printf("mod is a positive number\n");
                    return 1;
                }
                break;

            defalut:
                printf("Index %d is out of options\n", option_index);
            }
            break;

        case 'f':
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

    if (k == -1 || pnum == -1 || mod == -1)
    {
        printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
            argv[0]);
        return 1;
    }

    pthread_t threads[pnum];
    struct Segment segment[pnum];
    for (int i = 0; i < pnum; i++)
    {
        segment[i].begin = (k / pnum) * i + 1;
        segment[i].end = ((i != pnum - 1) ? (k / pnum) * (i + 1) : k);
        segment[i].mod = mod;
        if (pthread_create(&threads[i], NULL, (void*)factorial, (void*)(segment + i)) != 0)
        {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }

    for (int i = 0; i < pnum; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(1);
        }
    }
    printf("\nResult: %d\n", result);
    return 0;
}