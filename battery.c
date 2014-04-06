#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "battery.h"

int main()
{
	const int batMax = getValue(BAT_MAX);
	int batState = getValue(AC_CHRG);
	int batCur = 0;
	int batPre = 0;
	int batAvg = 0;
	int countIntervals = 0;

	while (1) {
		batCur = getValue(BAT_CUR);

		if (countIntervals++ == 0) {
			batPre = batCur;
		}
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
		if (batState) {
			printf("Charging: ");
			printf("%.f%\n", (float)batCur/batMax*100);
			if (batAvg) {
				printf("%.f%, ", (float)batCur/batMax*100);
				printf("estimated %.fmin left before fully charged\n",
					(float)(batMax - batCur) / batAvg / 60);
			}
		}
		else {
			printf("Discharging: ");
			printf("%.f%\n", (float)batCur/batMax*100);
			if (batAvg) {
				printf("%.f%, ", (float)batCur/batMax*100);
				printf("estimated %.fmin left\n", (float)batCur / batAvg / 60);
			}
		}

		sleep(POLL_INT);
	}

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

	return atoi(line);
}
