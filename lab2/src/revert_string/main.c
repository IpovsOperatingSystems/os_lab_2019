#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "revert_string.h"

int main(int argc, char *argv[])
{
	//argc - кол-во аргументов передаваемых в командной строке, argv массив аргументов
	if (argc != 2)
	{
		printf("Usage: %s string_to_revert\n", argv[0]);
		return -1;
	}

	// выделяем память под строку, она будет равна длине переданной строки + 1, где 1 - символ окончания строки
	char *reverted_str = malloc(sizeof(char) * (strlen(argv[1]) + 1));
	// далее копируем argv[1] в reverted_str
	strcpy(reverted_str, argv[1]);
	// инвертируем строку
	RevertString(reverted_str);

	printf("Reverted: %s\n", reverted_str);
	// освобждаем память, выделенную под строку
	free(reverted_str);
	return 0;
}

