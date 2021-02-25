#include "find_min_max.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//ye
struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

     for (int i = begin; i <= end; i++)
    {
       printf("%d\n", array[i]);
    }

    for (int i = begin; i <= end; i++)
    {
        if (array[i] >= min_max.max)
        {
            min_max.max = array[i];
        }

        if (array[i] <= min_max.min)
        {
            min_max.min = array[i];
        }
    }

    return min_max;
}
