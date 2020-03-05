#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "revert_string.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s string_to_revert\n", argv[0]);
		return -1;
	}

	unsigned arr_len = atoi(argv[1]);
	char* reverted_str = malloc(sizeof(char) * (arr_len + 1));
	strcpy(reverted_str, argv[2]);

	RevertString(reverted_str, arr_len);

	printf("Reverted: %s\n", reverted_str);
	free(reverted_str);
	return 0;
}


