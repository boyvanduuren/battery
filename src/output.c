#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "battery.h"
#include "output.h"
#include "main.h"

void replaceSubString(char **input, const char *oldval, const char *newval)
{
	char *result = calloc(1, sizeof(char) * BUF_SIZE * 2);
	strncpy(result, *input, BUF_SIZE);

	char *needle = strstr(result, oldval);
	int oldlen = strlen(oldval);

	if (needle == NULL) {
		free(result);
		return;
	}
	else {
		while (needle != NULL) {
			*needle = '\0';
			*(needle + oldlen - 1) = '\0';
			needle = needle + oldlen;

			char *temp = strdup(needle);
			strncat(result, newval, BUF_SIZE);
			strncat(result, temp, BUF_SIZE);
			free(temp);

			needle = strstr(result, oldval);
		}
	}
	sprintf(*input, result);
	free(result);
}

void printOutput(FILE *stream, battery *bat)
{
	// Set up initial string
	char *output = malloc(sizeof(char) * BUF_SIZE);
	snprintf(output, BUF_SIZE,
		"$STATE: $PERCENTAGE%%\n");

	// Replace variable stuff
	char placeholder[MAX_INTLEN];

	intToString(placeholder, getBatteryStatus(bat, "max"));
	replaceSubString(&output, "$MAX", placeholder);
	intToString(placeholder, getBatteryStatus(bat, "percentage"));
	replaceSubString(&output, "$PERCENTAGE", placeholder);
	if (getBatteryStatus(bat, "status") == 0) {
		replaceSubString(&output, "$STATE", "discharging");
	}
	else {
		replaceSubString(&output, "$STATE", "charging");
	}

	// Write to stream
	fprintf(stream, output);
	free(output);

	// Because I don't want to close and reopen the
	// filedescriptor to truncate the file, use ftruncate
	// and then flush the buffer to the file
	if (stream != stdout) {
		if (ftruncate(fileno(stream), 0) == -1) {
			fputs("Error truncating file", stderr);
			fclose(stream);
			exit(EXIT_FAILURE);
		}
		rewind(stream);
		if (fflush(stream) != 0) {
			perror("Couldn't flush buffer to file: ");
			exit(EXIT_FAILURE);
		}
	}
	else { // Flush stdout
		if (fflush(stream) != 0) {
			perror("Couldn't flush buffer to file: ");
			exit(EXIT_FAILURE);
		}
	}
}

void intToString(char *dest, int val)
{
	if (dest != NULL) {
		snprintf(dest, MAX_INTLEN + 1, "%d", val);
	}
}

