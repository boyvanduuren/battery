#ifndef MAIN_H

#define MAIN_H

#define BUF_SIZE 100
#define CHAR_ESC '\\'
#define CHAR_VAR '$'

FILE *fdout;
battery *bat;

int getValue(const char *filename);
void handleSignal();

#endif
