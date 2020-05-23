//
// Created by sergeyampo on 23.05.2020.
//

#include "sum.h"

long long int Sum(const struct SumArgs *args) {
    long long int sum = 0;
    // TODO: your code here

    for (int i = args->begin; i < args->end; i++) {
        sum += args->array[i];
    }

    return sum;
}

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *) args;
    return (void *) (long long int) Sum(sum_args);
}