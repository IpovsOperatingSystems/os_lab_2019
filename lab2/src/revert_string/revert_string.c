#include "revert_string.h"

void RevertString(char *str)
{
    int n = strlen(str);
    for (int i = 0, j = n - 1; i < j; i++, j--)
    {
        char ch = str[i];
        str[i] = str[j];
        str[j] = ch;
    }
}

