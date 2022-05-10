#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int X = 2;
int Y = 3;

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

void xy()
{
	int x;
	int y;
	pthread_mutex_lock(&mut1);
	x = X;
	printf("X = %d was put in th1\n", x);
	sleep(1);
	pthread_mutex_lock(&mut2);
	y = Y;
	printf("Y = %d was put in th1\n", y);
	printf("X*Y = %d\n", x * y);
	pthread_mutex_unlock(&mut2);
	pthread_mutex_unlock(&mut1);
}

void yx()
{
	int x;
	int y;
	pthread_mutex_lock(&mut2);
	y = Y;
	printf("Y = %d was put in th2\n", y);
	sleep(1);
	pthread_mutex_lock(&mut1);
	y = Y;
	printf("X = %d was put in th2\n", x);
	printf("Y*X = %d\n", y * x);
	pthread_mutex_unlock(&mut1);
	pthread_mutex_unlock(&mut2);
}

int main() {
	pthread_t thread1, thread2;

	if (pthread_create(&thread1, NULL, (void*)xy, NULL) != 0)
	{
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&thread2, NULL, (void*)yx, NULL) != 0)
	{
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(thread1, NULL) != 0)
	{
		perror("pthread_join");
		exit(1);
	}

	if (pthread_join(thread2, NULL) != 0)
	{
		perror("pthread_join");
		exit(1);
	}

	return 0;
}