#include<stdlib.h>
#include<stdio.h>

#include<errno.h>
#include <pthread.h>
#include<time.h>

#define COUNT_THREADS 2

int var[COUNT_THREADS];
pthread_mutex_t mutex[COUNT_THREADS];

void block(int* mutex_number);

int main() {
    pthread_t threads[COUNT_THREADS];

    for (int i = 0; i < COUNT_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, (void*)block, (void*) &i) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (int i = 0; i < COUNT_THREADS; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }
    printf("Mutex values: ");
    for (int i = 0; i < COUNT_THREADS; ++i){
        printf("%d ", var[i]);
    }
    printf("\nSuccessful completion of the program.\n");
    return 0;
}

void block(int* mutex_number){
    int x = (*mutex_number) % COUNT_THREADS;
    int y = (x + 1) % COUNT_THREADS;
    pthread_mutex_lock(&mutex[x]);
    var[x]++;
    // момент deadlock, пытаемся захватить другой мьютекс, но он может быть захвачен вторым потоком
    {
        printf("mutex[%d] has been locked, wait unlock mutex[%d]\n", x, y);
        pthread_mutex_lock(&mutex[y]);
        var[y]++;
        pthread_mutex_unlock(&mutex[y]);
        printf("mutex[%d] has been unlocked, next - mutex[%d]\n", y, x);
    }
    pthread_mutex_unlock(&mutex[x]);
}