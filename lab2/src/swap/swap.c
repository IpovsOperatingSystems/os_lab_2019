#include "swap.h"

void Swap(char* left, char* right)
{
    char tmp;
    tmp = *left;
    *left = *right;
    *right = tmp;
}

