#ifndef OUTPUT_H
#define OUTPUT_H

#define MAX_INTLEN 10

void intToString(char *dest, int val);
char findVariables(char *input);
void printOutput(FILE *stream, battery *bat);

#endif
