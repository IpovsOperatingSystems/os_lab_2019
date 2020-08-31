//
// Created by sergeyampo on 02.06.2020.
//
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>

void get_res_lock(pthread_mutex_t *mutexes) {
  pthread_mutex_lock(mutexes);
  sleep(1);
  printf("First thread locked first mutex and tried to lock second mutex\n");
  fflush(stdout);
  pthread_mutex_lock(mutexes + 1);
  pthread_mutex_unlock(mutexes);
  pthread_mutex_unlock(mutexes + 1);
}

void get_another_res(pthread_mutex_t *mutexes) {
  pthread_mutex_lock(mutexes + 1);
  sleep(1);
  printf("Second thread locked second mutex and tried to lock first mutex\n");
  fflush(stdout);
  pthread_mutex_lock(mutexes);
  pthread_mutex_unlock(mutexes);
  pthread_mutex_unlock(mutexes + 1);
}

int main() {
  pthread_mutex_t mutexes[2] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

  pthread_t threads[2];
  if (pthread_create(threads, NULL, (void *) get_res_lock, (void *) mutexes)) {
    perror("\nERROR CREATE THREAD\n");
    return 1;
  }

  if (pthread_create(threads + 1, NULL, (void *) get_another_res, (void *) mutexes)) {
    perror("\nERROR CREATE THREAD\n");
    return 1;
  }

  for (int i = 0; i < 2; ++i)
    pthread_join(threads[i], NULL);

  return 0;
}

