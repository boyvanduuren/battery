#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "battery.h"

int main(int argc, char *argv[])
{
	// TODO: better way of storing battery values
	const int batMax = getValue(BAT_MAX);
	int batState = getValue(AC_CHRG);
	int batCur = 0;
	int batPre = 0;
	int batAvg = 0;
	int countIter = 0;
	FILE *fdout;

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
		batCur = getValue(BAT_CUR);

		// On the first iteration, or after a
		// battery change, set batPre to the
		// current value
		if (countIter == 0) {
			batPre = batCur;
		}
		// Calculate average increase/decrease
		// of battery when defined iterations
		// are done
		else if (countIter % POLL_ITR == 0) {
			batAvg = (batPre - batCur) / POLL_AVG;
			batPre = batCur;
		}
		// If batState changed between discharging and charging
		// reset average counters
		if (batState != getValue(AC_CHRG)) {
			countIter = 0;
			batAvg = 0;
		}

		batState = getValue(AC_CHRG);
		printOutput(fdout, batCur, batMax, batAvg, batState);
		// Because I don't want to close and reopen the
		// filedescriptor to truncate the file, use ftruncate
		// and then flush the buffer to the file
		if (fdout != stdout) {
			if (ftruncate(fileno(fdout), 0) == -1) {
				fputs("Error truncating file", stderr);
				fclose(fdout);
				exit(EXIT_FAILURE);
			}
			rewind(fdout);
			if (fflush(fdout) != 0) {
				perror("Couldn't flush buffer to file: ");
				exit(EXIT_FAILURE);
			}
		}

		sleep(POLL_INT);
		countIter++;
	}

	fclose(fdout);
	return 0;
}

// Read file content
int getValue(const char *filename) {
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
		fputs("Error reading file", stderr);
		exit(EXIT_FAILURE);
	}

	close(fd);
	return atoi(line);
}

// Write output to filedescriptor
// TODO: make output string user formattable
void printOutput(FILE *stream, int batCur, int batMax, int batAvg, int batState)
{
	if (batState) {
		if (batAvg) {
			fprintf(stream, "Charging: ");
			fprintf(stream, "%.f%%, ", (float)batCur/batMax*100);
			fprintf(stream, "estimated %.fmin left before fully charged\n",
				(float)(batMax - batCur) / -batAvg / 60);
		}
		else {
			fprintf(stream, "Charging: ");
			fprintf(stream, "%.f%%\n", (float)batCur/batMax*100);
		}
	}
	else {
		if (batAvg) {
			fprintf(stream, "Discharging: ");
			fprintf(stream, "%.f%%, ", (float)batCur/batMax*100);
			fprintf(stream, "estimated %.fmin left\n", (float)batCur / batAvg / 60);
		}
		else {
			fprintf(stream, "Discharging: ");
			fprintf(stream, "%.f%%\n", (float)batCur/batMax*100);
		}
	}
}
