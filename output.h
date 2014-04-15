#ifndef OUTPUT_H
#define OUTPUT_H

#define BUF_SIZE 100
#define CHAR_ESC '\\'
#define CHAR_VAR '$'

int findVariables(char *input);
char *replaceSubString(const char *input, const char *oldval, const char *newval);

#endif
