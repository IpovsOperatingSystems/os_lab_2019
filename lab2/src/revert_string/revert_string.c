#include "revert_string.h"
#include <string.h>
#include <stdlib.h>

void RevertString(char *str)
{
	int len = strlen(str);
	char *reverted_str = malloc(sizeof(char) * len + 1);
	strcpy(reverted_str, str);
	for (int i = len - 1; i > -1; i--)
	{
		reverted_str[i] = str[(len - 1) - i];
	}
	strcpy(str, reverted_str);
}

