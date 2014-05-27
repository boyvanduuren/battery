#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "battery.h"

char *intToString(int val)
{
	int valLength;
	char *out;

	if (val != 0) {
		// Length of int as string
		valLength = floor(log10(abs(val))) + 1;
	}
	else {
		valLength = 1;
	}

	out = malloc(sizeof(char) * valLength);

	if (out != NULL) {
		snprintf(out, valLength + 1, "%d", val);
	}

	return out;
}
