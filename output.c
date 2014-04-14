#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "output.h"

// TODO: Think of valid names and enforce
// sets of chars like $$ are now counted
// as two counts
int findVariables(char *input)
{
	char *needle = strchr(input, CHAR_VAR);
	int counter = 0;

	while (needle != NULL) {
		needle--;
		if (*needle != CHAR_ESC) {
			needle = needle + 2;
			counter++;
		}
		needle = strchr(needle, CHAR_VAR);
	}
	return counter;
}

char *replaceSubString(const char *input, const char *oldval, const char *newval)
{
	char *result = malloc(sizeof(char) * BUF_SIZE * 2);
	strncpy(result, input, BUF_SIZE);

	char *needle = strstr(result, oldval);
	int oldlen = strlen(oldval);

	if (needle == NULL) {
		return result;
	}
	else {
		while (needle != NULL) {
			*needle = '\0';
			*(needle + oldlen - 1) = '\0';
			needle = needle + oldlen;

			char *temp = strdup(needle);
			strncat(result, newval, BUF_SIZE);
			strncat(result, temp, BUF_SIZE);

			needle = strstr(result, oldval);
		}
	}
	return result;
}
