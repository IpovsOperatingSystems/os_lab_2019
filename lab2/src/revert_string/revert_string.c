#include "revert_string.h"

void RevertString(char *str)
{
	int len = 0;
  char* tmp_str = str;
  while (*tmp_str != '\0')
  {
    len++;
    tmp_str++;
  }
  
  tmp_str = 0;
  for(int i = 0; i < (len/2); i++)
  {
    char tmp = *(str + i);
    *(str + i) = *(str + len - i - 1);
    *(str + len - i - 1) = tmp;
  }
}