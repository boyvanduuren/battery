#define MAX_LINE 100
#define INTERVAL 1
#define BAT_CUR "/sys/class/power_supply/BAT0/charge_now"
#define BAT_MAX "/sys/class/power_supply/BAT0/charge_full"
#define AC_CHRG "/sys/class/power_supply/AC/online"

int getValue(const char *filename);
