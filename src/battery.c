#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battery.h"

int getBatteryStatus(battery *bat, const char *field)
{
	if (strcmp("state", field) == 0) {
		return bat->state;
	}
	else if (strcmp("max", field) == 0) {
		return bat->max;
	}
	else if (strcmp("current", field) == 0) {
		return bat->cur;
	}
	else if (strcmp("previous", field) == 0) {
		return bat->pre;
	}
	else if (strcmp("average", field) == 0) {
		return bat->avg;
	}
	else if (strcmp("percentage", field) == 0) {
		float percentage = (float)bat->cur/bat->max*100;
		return (int)(percentage + 0.5);
	}
	return -1;
}
