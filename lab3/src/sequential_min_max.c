#include <stdio.h>
#include <stdlib.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }

  //преобразует аргумент seed в целое число и проверяет, что оно положительно.
  int seed = atoi(argv[1]);
  if (seed <= 0) {
    printf("seed is a positive number\n");
    return 1;
  }

  //Преобразует аргумент arraysize в целое число и проверяет, что оно положительно.
  int array_size = atoi(argv[2]);
  if (array_size <= 0) {
    printf("array_size is a positive number\n");
    return 1;
  }

  //Выделяет память для массива array размером arraysize элементов с
  int *array = malloc(array_size * sizeof(int));

  //Генерирует массив с помощью функции GenerateArray, используя переданный seed.
  GenerateArray(array, array_size, seed);

  //Вызывает функцию GetMinMax для нахождения минимального и максимального элементов
  struct MinMax min_max = GetMinMax(array, 0, array_size);

  //Освобождает память, выделенную для массива array.
  free(array);

  printf("min: %d\n", min_max.min);
  printf("max: %d\n", min_max.max);

  return 0;
}
