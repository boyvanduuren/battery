#ifndef OUTPUT_H
#define OUTPUT_H

char *intToString(int val);
char findVariables(char *input);

char *formatOutput(char *fmt, battery *bat);
void printOutput(FILE *stream, battery *bat);

#endif
