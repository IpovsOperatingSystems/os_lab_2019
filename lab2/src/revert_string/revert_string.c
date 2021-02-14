#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "revert_string.h"

void RevertString(char *str)
{
	int i = 0;
	int len = strlen(str);
	while (i <= strlen(str) / 2 - 1)
	{
		char tmp = *(str + i);
		*(str + i) = *(str + len -1 - i);
		*(str + len -1 - i) = tmp;
		i++;
	}
}

