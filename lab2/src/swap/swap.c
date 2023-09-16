#include "swap.h"

void Swap(char *left, char *right)
{
	char c = *left;
	*left = *right;
	*right = c;
}

