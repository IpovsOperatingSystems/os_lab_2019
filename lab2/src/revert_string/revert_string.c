#include "revert_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void RevertString(char *str)
{
int len=strlen(str) - 1;
char *new_str=(char*)malloc(sizeof(char)*len);
int i;
for(i = 0; i <= len; i++){
new_str[i]=str[len-i];
//printf("\ntest: %c %c\n", str[len-i], new_str[i]);
}
strcpy(str, new_str);
//printf("\nnew: %s\n", str);
free(new_str);
}

