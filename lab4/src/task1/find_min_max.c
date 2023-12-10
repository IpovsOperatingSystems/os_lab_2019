#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;
    int i = 0;
    for (i = begin; i < end; i++) {
        int value = array[i]; 
        if (value < min_max.min) {
            min_max.min = value;
        }
        if (value > min_max.max) {
            min_max.max = value;
        }
    }
    return min_max;
}
