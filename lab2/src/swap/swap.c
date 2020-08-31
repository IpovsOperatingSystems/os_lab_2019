#include "swap.h"

void Swap(char *left, char *right)
{
	char temp = *left;
    *left = *right;
    *right = temp;
}
