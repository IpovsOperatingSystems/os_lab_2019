#include <pthread.h>
#include <stdio.h>

pthread_mutex_t A_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t B_mutex = PTHREAD_MUTEX_INITIALIZER;

static int A = 1;
static int B = 1;

void *deadlock_A(void *args) {
  pthread_mutex_lock(&A_mutex);
  A++;
  B++;
}

int main() {

  pthread_t threads[2];

  pthread_create(&threads[0], NULL, deadlock_A, NULL);
  pthread_create(&threads[1], NULL, deadlock_A, NULL);

  for (int i = 0; i < 2; i++) {
    pthread_join(threads[i], NULL);
  }
  
  printf("First - %i\nSecond - %i\n", A, B);
  return 0;
}
