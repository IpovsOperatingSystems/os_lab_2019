#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = begin; i < end; i++)
  {

	if (min_max.min > array[i])
		min_max.min =  array[i];

	if (min_max.max < array[i])
		min_max.max = array[i];
  }

  return min_max;
}

