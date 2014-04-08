#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 100
#define CHAR_ESC '\\'
#define CHAR_VAR '$'

// Small program that replaces keywords in a string

int main(int argc, char *argv[])
{
    char *input = argv[1];
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

    return 0;
}
