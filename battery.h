#define MAX_LINE 100
#define POLL_INT 5
#define POLL_AVG 20
#define POLL_ITR (POLL_AVG / POLL_INT)
#define BAT_CUR "/sys/class/power_supply/BAT0/charge_now"
#define BAT_MAX "/sys/class/power_supply/BAT0/charge_full_design"
#define AC_CHRG "/sys/class/power_supply/AC/online"

typedef struct batteries {
	int max;
	int state;
	int cur;
	int pre;
	int avg;
} battery;

FILE *fdout;

int getValue(const char *filename);
void printOutput(FILE *stream, int batCur, int batMax, int batAvg, int batState);
void handleSignal();
