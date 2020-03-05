#include "revert_string.h"

void RevertString(char* str, const unsigned arr_len) {
	unsigned medium_index = arr_len / 2;
	for (int i = 0; i < medium_index; ++i) {
		char temp = str[i];
		str[i] = str[arr_len - i - 1];
		str[arr_len - i - 1] = temp;
	}
}
