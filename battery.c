#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "battery.h"

int main(int argc, char *argv[])
{
	const int batMax = getValue(BAT_MAX);
	int batState = getValue(AC_CHRG);
	int batCur = 0;
	int batPre = 0;
	int batAvg = 0;
	int countIntervals = 0;
	FILE *fdout;

	if (argc == 3 && argv[1][1] == 'w') {
		fdout = fopen(argv[2], "w");
		if (fdout == NULL) {
			fprintf(stderr, "Error opening %s: ", argv[2]);
			perror("");
			exit(1);
		}
	}
	else {
		fdout = stdout;
	}

	while (1) {
		batCur = getValue(BAT_CUR);

		if (countIntervals == 0) {
			batPre = batCur;
		}
		// Calculate average increase/decrease
		// of battery when defined iterations
		// are done
		else if (countIntervals % POLL_ITR == 0) {
			batAvg = (batPre - batCur) / POLL_AVG;
			batPre = batCur;
		}
		// If batState changed between discharging and charging
		// reset average counters
		if (batState != getValue(AC_CHRG)) {
			countIntervals = 0;
			batAvg = 0;
		}

		batState = getValue(AC_CHRG);
		printOutput(fdout, batCur, batMax, batAvg, batState);
		if (fdout != stdout) {
			if (ftruncate(fileno(fdout), 0) == -1) {
				fputs("Error truncating file", stderr);
				fclose(fdout);
				exit(1);
			}
			rewind(fdout);
			fflush(fdout);
		}

		sleep(POLL_INT);
		countIntervals++;
	}

	fclose(fdout);
	return 0;
}

int getValue(const char *filename) {
	int fd;
	char line[MAX_LINE];
	ssize_t n;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		fputs("Error opening file", stderr);
		exit(1);
	}

	n = read(fd, line, MAX_LINE);
	if (n == -1) {
		fputs("Error reading file", stderr);
		exit(1);
	}

	close(fd);
	return atoi(line);
}

void printOutput(FILE *stream, int batCur, int batMax, int batAvg, int batState)
{
	if (batState) {
		if (batAvg) {
			fprintf(stream, "Charging: ");
			fprintf(stream, "%.f%, ", (float)batCur/batMax*100);
			fprintf(stream, "estimated %.fmin left before fully charged\n",
				(float)(batMax - batCur) / -batAvg / 60);
		}
		else {
			fprintf(stream, "Charging: ");
			fprintf(stream, "%.f%\n", (float)batCur/batMax*100);
		}
	}
	else {
		if (batAvg) {
			fprintf(stream, "Discharging: ");
			fprintf(stream, "%.f%, ", (float)batCur/batMax*100);
			fprintf(stream, "estimated %.fmin left\n", (float)batCur / batAvg / 60);
		}
		else {
			fprintf(stream, "Discharging: ");
			fprintf(stream, "%.f%\n", (float)batCur/batMax*100);
		}
	}
}
