#include <stdio.h>
#include <stdlib.h>
#include "find_min_max.h"
#include "utils.h"

int main() {
    const size_t size = 10;

    int* array = (int*)malloc(sizeof(int) * size);
    GenerateArray(array, size, 12);
    struct MinMax min_max = GetMinMax(array, 0, size-1);

    printf("Array is: ");
    for(size_t i = 0; i < size; ++i){
        printf("%d, ", array[i]);
    }
    printf("\nMinimum is: %d, maximum is: %d", min_max.min, min_max.max);

    return 0;
}
