#include "revert_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void RevertString(char *str)
{
	int length = strlen(str) + 1;
	
	char* cur = str;
	char* tmp = malloc(sizeof(char) * length);
	
	strcpy(cur, str);
	
	tmp += length - 1;
	*tmp = '\0';
	do {
		* --tmp = *cur;
	}	
	while (* ++cur != '\0');
	
	strcpy(str, tmp);
	free(tmp);
}

