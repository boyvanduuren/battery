#ifndef MAIN_H

#define MAIN_H

#define BUF_SIZE 100
#define CHAR_ESC '\\'
#define CHAR_VAR '$'

FILE *fdout;
battery *bat;
char message[BUF_SIZE];

int getValue(const char *filename);
void handleSignal();
void handleOptions(int argc, char *argv[]);

typedef enum __operation_t {
	OP_MESSAGE = 1,
	OP_FILE = (1 << 1)
} operation_t;

operation_t opmask;

#endif
