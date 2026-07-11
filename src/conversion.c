#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    PERIOD_2,
    PERIOD_3,
    PERIOD_4,
    PERIOD_5,
    PERIOD_6,
}Periods;

int stringWeekToNumber(char* weekday) {
    if (strncmp(weekday, "Mon", FIRST_COMPARISON_CHARS) == 0)
        return MONDAY;
    else if (strncmp(weekday, "Tue", FIRST_COMPARISON_CHARS) == 0)
        return TUESDAY;
    else if (strncmp(weekday, "Wed", FIRST_COMPARISON_CHARS) == 0)
        return WEDNESDAY;
    else if (strncmp(weekday, "Thu", FIRST_COMPARISON_CHARS) == 0)
        return THURSDAY;
    else if (strncmp(weekday, "Fri", FIRST_COMPARISON_CHARS) == 0)
        return FRIDAY;
    else if (strncmp(weekday, "Sat", FIRST_COMPARISON_CHARS) == 0)
        return SATURDAY;
    else if (strncmp(weekday, "Sun", FIRST_COMPARISON_CHARS) == 0)
        return SUNDAY;
    else {
        printf("'%s' does not match with any known weekday!\n", weekday);
        return -1;
    }
}

char* stringNumberToWeek(int weekday) {
    switch (weekday) {
        case MONDAY:
            return "Mon";
        case TUESDAY:
            return "Tue";
        case WEDNESDAY:
            return "Wed";
        case THURSDAY:
            return "Thu";
        case FRIDAY:
            return "Fri";
        case SATURDAY:
            return "Sat";
        case SUNDAY:
            return "Sun";
        default:
            return "";
    }
}
