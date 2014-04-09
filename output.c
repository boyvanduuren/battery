#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "output.h"

// I might not even need this, but I'm leaving it just in case
void findVariables(char *input)
{
	char *needle = strchr(input, CHAR_VAR);
	char *varname = malloc(sizeof(char) * BUF_SIZE);
	int i;

	if (needle != NULL) {
		needle--;
		if (needle[0] == CHAR_ESC) {
			printf("%c is escaped, we won't do anything\n", CHAR_VAR);
		}
		else {
			needle = needle + 2;
			char *step = strchr(needle, ' ');
			strncpy(varname, needle, step - needle);

			if (!strcmp(varname, "AVG")) {
				printf("replace with average\n");
				needle--;
				needle[0] = '\0';
				step[0] = '\0';
				step++;
				printf("%s(average)%s", input, step);
			}
			else {
				printf("couldn't find anything to replace %s with\n",
					varname);
			}
		}
	}
	else {
		printf("%s\n", input);
	}
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
