#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "battery.h"

int main()
{
	int batCur = 0;
	int batPre = 0;
	int batMax = 0;
	int batDecAvg = 0;
	int countIntervals = 0;

	batMax = getValue(BAT_MAX);

	while (1) {
		countIntervals++;
		batPre = batCur;
		batCur = getValue(BAT_CUR);

		if (batPre > 0) {
			batDecAvg = (batPre - batCur) / countIntervals;
		}

		if (getValue(AC_CHRG)) {
			printf("Charging: ");
			printf("%d average increase per %d sec\n", -batDecAvg, INTERVAL);
			printf("That means: %.f * min left\n\n",
				(float)(batMax - batCur) / (batDecAvg * (60 / INTERVAL)), INTERVAL);
		}
		else {
			printf("Discharging: ");
			printf("%d average decrease per %d sec\n", batDecAvg, INTERVAL);
			printf("That means: %.f * min left\n\n",
				(float)batCur / (batDecAvg * (60 / INTERVAL)), INTERVAL);
		}

		sleep(INTERVAL);
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

float remainingTime(int avg, int remaining)
{
	return (float)remaining / avg;
}
