#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "battery.h"
#include "output.h"
#include "main.h"

int main(int argc, char *argv[])
{
	bat = malloc(sizeof(battery));
	bat->max = getValue(BAT_MAX);
	bat->state = getValue(AC_CHRG);
	bat->cur = 0;
	bat->pre = 0;
	bat->avg = 0;

	int countIter = 0;

	// Handle SIGINT, so we can gracefully exit
	signal(SIGINT, handleSignal);

	// TODO: Improve option handling!
	if (argc == 3 && argv[1][1] == 'w') {
		// If -w file is given, open file
		fdout = fopen(argv[2], "w");
		if (fdout == NULL) {
			fprintf(stderr, "Error opening %s: ", argv[2]);
			perror("");
			exit(EXIT_FAILURE);
		}
	}
	else {
		// else, just write to stdout
		fdout = stdout;
	}

	while (1) {
		bat->cur = getValue(BAT_CUR);

		// On the first iteration, or after a
		// battery change, set bat->pre to the
		// current value
		if (countIter == 0) {
			bat->pre = bat->cur;
		}
		// Calculate average increase/decrease
		// of battery when defined iterations
		// are done
		else if (countIter % POLL_ITR == 0) {
			bat->avg = (bat->pre - bat->cur) / POLL_AVG;
			bat->pre = bat->cur;
		}
		// If bat->state changed between discharging and charging
		// reset average counters
		if (bat->state != getValue(AC_CHRG)) {
			countIter = 0;
			bat->avg = 0;
		}
		bat->state = getValue(AC_CHRG);

		printOutput(fdout, bat);
		sleep(POLL_INT);
		countIter++;
	}

	return 0;
}

// Read file content
int getValue(const char *filename)
{
	int fd;
	char line[MAX_LINE];
	ssize_t n;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		fputs("Error opening file", stderr);
		exit(EXIT_FAILURE);
	}

	n = read(fd, line, MAX_LINE);
	if (n == -1) {
		fputs("Error opening file", stderr);
		exit(EXIT_FAILURE);
	}

	close(fd);
	return atoi(line);
}

// Handle SIGINT
void handleSignal() {
	fclose(fdout);
	free(bat);
	exit(EXIT_SUCCESS);
}
