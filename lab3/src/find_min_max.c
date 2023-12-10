#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

  for (int i = begin; i < end; i++) {
      int value = array[i];
      if (value > min_max.max) {
          min_max.max = i;
      }

      if (value < min_max.min) {
          min_max.min = i;
      }
  }


  return min_max;
}
