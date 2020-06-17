//
// Created by sergeyampo on 17.06.2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <inttypes.h>
#include "./num_partition/number_partition.h"


const uint64_t factorial = 20;

const uint64_t threads_amount = 4;

const uint64_t multiply_by_interval(const struct Interval* interval) {
  if (interval->begin == interval->end)
	return interval->begin;

  uint64_t answer = interval->begin;
  for (uint64_t i = interval->begin + 1; i <= interval->end; ++i) {
	answer *= i;
  }

  return answer;
}

int main() {
  if (factorial < 1 || threads_amount < 1) {
	printf("Factorial and threads cannot be less that 1!");
	return 1;
  }

  struct Interval* intervals = partition_num_to_intervals(&factorial, threads_amount);
  uint64_t used_threads = threads_amount;
  if (threads_amount > factorial) {
	used_threads = factorial;
  }

  clock_t start = clock();

  pthread_t threads[used_threads];
  for (uint64_t i = 0; i < used_threads; ++i) {
	if (pthread_create(&threads[i], NULL, &multiply_by_interval, intervals + i) != 0) {
	  printf("Error while creating a thread!");
	  return 1;
	}
  }

  uint64_t parts[used_threads];
  uint64_t answer = 1;

  for (uint64_t i = 0; i < used_threads; ++i) {
	void* part_res;
	pthread_join(threads[i], &part_res);
	parts[i] = (uint64_t)part_res;
	answer *= parts[i];
  }

  clock_t end = clock();

  printf("%" PRIu64 "\n", answer);

  printf("To calculate factorial %d by %d threads takes %F seconds\n",
		 factorial,
		 used_threads,
		 (float)(end - start) / CLOCKS_PER_SEC);

  return 0;
}
