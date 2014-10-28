#ifndef MAIN_H

#define MAIN_H

#define BUF_SIZE 100
#define CHAR_ESC '\\'
#define CHAR_VAR '$'

FILE *fdout;
battery *bat;
char message[BUF_SIZE];
char binaryname [256];
unsigned int intervalOverride;
unsigned int averageOverride;

int getValue(const char *filename);
void handleSignal();
void handleOptions(int argc, char *argv[]);

typedef enum __operation_t {
	OP_MESSAGE = 1,
	OP_FILE = (1 << 1),
	OP_INTERVAL = (1 << 2),
	OP_AVERAGE = (1 << 3)
} operation_t;

operation_t opmask;

#endif
