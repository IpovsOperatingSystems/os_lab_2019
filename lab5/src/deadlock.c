#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <getopt.h>

#define THREAD_NUM 8

pthread_mutex_t mutexFuel;
pthread_mutex_t mutexFuel_2;
int fuel = 50;

void* do_something(void* args)
{
    pthread_mutex_lock(&mutexFuel_2);//deadlock
    fuel += 50;
    pthread_mutex_t mutexFuel;
    printf("Incremented fuel to: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
    
}

void* do_something_2(void* args)
{
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);//deadlock
    fuel += 50;
    pthread_mutex_t mutexFuel_2;
    printf("Incremented fuel to: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char* argv[])
{
    pthread_t pt[THREAD_NUM];
    pthread_mutex_init(&mutexFuel, NULL);

    for (int i =0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&pt[i], NULL, &do_something, NULL) != 0)
        {
            perror("Error while creating thread");
        }
    }

    for (int i =0; i < THREAD_NUM; i++)
    {
        if (pthread_join(pt[i], NULL) != 0)
        {
            perror("Error while joining");
        }
    }
    printf("Fuel: %d\n", fuel);
    pthread_mutex_destroy(&mutexFuel);
}