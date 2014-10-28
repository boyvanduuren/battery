#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include "battery.h"
#include "output.h"
#include "main.h"

int main(int argc, char *argv[])
{
	// Initialize variables
	strncpy(binaryname, argv[0], 256);
	bat = malloc(sizeof(battery));
	bat->max = getValue(BAT_MAX);
	bat->state = getValue(AC_CHRG);
	bat->cur = 0;
	bat->pre = 0;
	bat->avg = 0;
	int countIter = 0;
	int interval;
	int average;
	int iterations;
	fdout = stdout;
	strcat(message, "$STATE: $PERCENTAGE%%\n");

	// Handle options
	handleOptions(argc, argv);

	// Handle SIGINT, so we can gracefully exit
	signal(SIGINT, handleSignal);

	// We need to check if the user has given either
	// option -i or -a to override the interval or average
	// if so, we need use their values in stead
	// TODO: create method to check if a certain option is used
	if (((opmask >> 2) & 1) == 1) {
		interval = intervalOverride;
	}
	else {
		interval = POLL_INT;
	}
	if (((opmask >> 3) & 1) == 1) {
		average = averageOverride;
	}
	else {
		average = POLL_AVG;
	}
	// Average should always be equal to, or higher than interval
	if (average >= interval) {
		iterations = average/interval;
	}
	else {
		printf("%s: average needs to be higher than, " \
			"or equal to interval\n", binaryname);
		printf("Try '%s --help' for more information\n", binaryname);
		exit(EXIT_FAILURE);
	}

	while (1) {
		// Get our first battery value
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
		else if (countIter % iterations == 0) {
			bat->avg = (bat->pre - bat->cur) / average;
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
		sleep(interval);
		countIter++;
	}

	return 0;
}

void printUsage() {
	printf("Usage: %s OPTION...\n" \
		"Print the average in- or decrease of battery charge. \n \n" \
		"Mandatory arguments to long options are mandatory for short options too. \n" \
		"  -w, --write=FILE      write output to file, in stead of STDOUT \n" \
		"  -f, --format=FORMAT   format the output using keywords. \n" \
		"                        "  \
		"  keywords:  $STATE: outputs either  \"charging \" or  \"discharging \" \n" \
		"                        "  \
		"  $PERCENTAGE: outputs the battery charge in percentages \n" \
		"                        "  \
		"  $MIN: shows the battery's minimum charge as an integer (always 0) \n" \
		"                        "  \
		"  $MAX: shows the battery's maximum charge as an integer \n" \
		"                        "  \
		"  $CUR: shows the battery's current charge as an integer \n" \
		"  -i, --interval=NUM    change how often we output the status \n" \
		"                          the default for this is 5; interval should always \n" \
		"                          be lower in value than the average \n" \
		"  -a, --average=NUM     change how long we should wait before calculating \n" \
		"                          an average change in battery charge; the default \n" \
		"                          for this is 20; has to be higher in value than interval \n \n" \
		"  -h, --help     display this help and exit \n" \
		"  -v, --version  display version information \n", binaryname);

	exit(EXIT_FAILURE);
}

void printVersion() {
	printf("battery %d.%d.%d\n", MAJOR, MINOR, PATCH);
	printf("Copyright (c) 2014 B.C. van Duuren\n");
	printf("License MIT: http://opensource.org/licenses/MIT\n\n");
	printf("Written by B.C. van Duuren\n");

	exit(EXIT_SUCCESS);
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
void handleSignal()
{
	fclose(fdout);
	free(bat);
	exit(EXIT_SUCCESS);
}

// Open file so we can write to it later
void handleOptionFile(char *file)
{
	fdout = fopen(file, "w");
}

// Copy custom message for later use
void handleOptionMessage(char *custommessage)
{
	strncpy(message, custommessage, BUF_SIZE - 2);
	strcat(message, "\n");
}

// Verify interval, then copy it for later use
void handleOptionInterval(char *interval)
{
	int i;
	for (i = 0; interval[i] != '\0'; i++) {
		if (!isdigit(interval[i])) {
			printf("%s: argument should be numerical\n", binaryname);
			exit(EXIT_FAILURE);
		}
	}
	intervalOverride = atoi(interval);
}

// Verify average, then copy it for later use
void handleOptionAverage(char *average)
{
	int i;
	for (i = 0; average[i] != '\0'; i++) {
		if (!isdigit(average[i])) {
			printf("%s: argument should be numerical\n", binaryname);
			exit(EXIT_FAILURE);
		}
	}
	averageOverride = atoi(average);
}

void handleOptions(int argc, char *argv[])
{
	int opt = 0, long_index = 0;
	static struct option long_options[] = {
		{"write", required_argument, 0, 'w'},
		{"format", required_argument, 0, 'f'},
		{"interval", required_argument, 0, 'i'},
		{"average", required_argument, 0, 'a'},
		{"help", required_argument, 0, 'h'},
		{"version", required_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "w:m:i:a:hv",
			long_options, &long_index)) != -1) {
		switch (opt) {
			case 'w':
				opmask |= OP_FILE;
				handleOptionFile(optarg);
				break;
			case 'm':
				opmask |= OP_MESSAGE;
				handleOptionMessage(optarg);
				break;
			case 'i':
				opmask |= OP_INTERVAL;
				handleOptionInterval(optarg);
				break;
			case 'a':
				opmask |= OP_AVERAGE;
				handleOptionAverage(optarg);
				break;
			case 'h':
				printUsage(argv[0]);
			case 'v':
				printVersion();
			default:
				printUsage();
		}
	}
}
