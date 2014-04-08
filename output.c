#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "output.h"

// Small program that replaces keywords in a string

int main(int argc, char *argv[])
{
    char *input = argv[1];
//    findVariables(input);
    printf("%s\n", replaceSubString(input, "$AVG", "3.14"));

    return 0;
}

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

// TODO: this works, but only replaces the first occurence of oldval
char *replaceSubString(const char *input, const char *oldval, const char *newval)
{
    char *haystack = strdup(input);
    char *needle = strstr(haystack, oldval);
    char *result = malloc(sizeof(char) * BUF_SIZE);
    int repl_len = strlen(oldval);

    if (needle == NULL) {
        return haystack;
    }
    else {
        needle[0] = '\0';
        *(needle + repl_len - 1) = '\0';
        needle = needle + repl_len;
        strncat(result, haystack, BUF_SIZE);
        strncat(result, newval, BUF_SIZE);
        strncat(result, needle, BUF_SIZE);
        return result;
    }
}
