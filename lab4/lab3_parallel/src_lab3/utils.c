#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  int i;
  for (i = 0; i < array_size; i++) {
    array[i] = rand();
  }
}
