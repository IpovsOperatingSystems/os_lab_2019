#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <getopt.h>
#include <string.h>

void do_one_thing(int *);
void do_another_thing(int *);
void do_wrap_up(int);
int common = 0;
bool with_mutex = false;
int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv) {
    while (1) {
        static struct option options[] = {
            {"mutex", no_argument, 0, 'Y'},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        int c = getopt_long(argc, argv, "Y", options, &option_index);
        if (c == -1){ 
            break; 
        } else if (c == 'Y') { 
            with_mutex = true; 
            break;
        } else {
            printf("Usage: %s --mutex", argv[0]);
            return 1;
        } 
    }
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, (void *)do_one_thing, (void *)&common) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, (void *)do_another_thing, (void *)&common) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }

    do_wrap_up(common);

    return 0;
}

void do_one_thing(int *pnum_times) {
    int i, j, x;
    unsigned long k;
    int work;
    for (i = 0; i < 50; i++) {
        if (with_mutex) pthread_mutex_lock(&mut);
        printf("doing one thing\n");
        work = *pnum_times;
        printf("counter = %d\n", work);
        work++;
        for (k = 0; k < 500000; k++)
        ;                
        *pnum_times = work;
        if (with_mutex) pthread_mutex_unlock(&mut);
    }
}

void do_another_thing(int *pnum_times) {
    int i, j, x;
    unsigned long k;
    int work;
    for (i = 0; i < 50; i++) {
        if (with_mutex) pthread_mutex_lock(&mut);
        printf("doing another thing\n");
        work = *pnum_times;
        printf("counter = %d\n", work);
        work++;
        for (k = 0; k < 500000; k++)
        ;
        *pnum_times = work;
        if (with_mutex) pthread_mutex_unlock(&mut);
    }
}

void do_wrap_up(int counter) {
    int total;
    printf("All done, counter = %d\n", counter);
}
