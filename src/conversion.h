#ifndef CONVERSION
#define CONVERSION

#define FIRST_COMPARISON_CHARS 3

typedef enum {
    MONDAY = 0,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
}Weekdays;

typedef enum {
    PERIOD_1 = 0,
    PERIOD_2 = 0,
    PERIOD_3 = 0,
    PERIOD_4 = 0,
    PERIOD_5 = 0,
    PERIOD_6 = 0,
}Periods;

int stringWeekToNumber(char* weekday);
char* stringNumberToWeek(int weekday);

#endif
