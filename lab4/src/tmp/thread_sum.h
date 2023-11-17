#ifdef THREAD_SUM_H
#define THREAD_SUM_H

struct SumArgs {
    int* array;
    int begin;
    int end;
};

int Sum(const struct SumArgs *args);

void* ThreadSum(void *args);

#endif