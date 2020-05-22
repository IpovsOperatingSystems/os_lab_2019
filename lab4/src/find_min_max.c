#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int* array, unsigned int begin, const unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

    while(begin <= end)
    {
        /* If current element is greater than max */
        if(array[begin] > min_max.max)
        {
            min_max.max = array[begin];
        }

        /* If current element is smaller than min */
        if(array[begin] < min_max.min){
            min_max.min = array[begin];
        }
        ++begin;
    }

    return min_max;
}
