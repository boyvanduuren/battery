#ifndef MAIN_H

#define MAIN_H

#include <limits.h>

#define BUF_SIZE 100
#define CHAR_ESC '\\'
#define CHAR_VAR '$'

typedef enum __operation_t {
	OP_MESSAGE = 1,
	OP_FILE = (1 << 1),
	OP_INTERVAL = (1 << 2),
	OP_AVERAGE = (1 << 3)
} operation_t;

char *bat_max;
char *bat_cur;
FILE *fdout;
battery *bat;
char message[BUF_SIZE];
char binaryname[PATH_MAX];
unsigned int intervalOverride;
unsigned int averageOverride;

const char *findBattery();
int getValue(const char *filename);
int isOptionSet(int option);
void handleSignal();
void handleOptions(int argc, char *argv[]);

operation_t opmask;

#endif
