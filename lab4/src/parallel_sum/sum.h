//
// Created by sergeyampo on 23.05.2020.
//

#ifndef ZOMBIES_C_H
#define ZOMBIES_C_H

struct SumArgs {
    int *array;
    int begin;
    int end;
};

long long int Sum(const struct SumArgs *args);

void *ThreadSum(void *args);

#endif //ZOMBIES_C_H
