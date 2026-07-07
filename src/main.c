// IMPORTANT: please keep main.c as small as possible;
// everything here is just placeholder as a proof of 
// concept at the time being (I do mean EVERYTHING)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structs.h"

#define MAX_LENGTH 128

int main() {
    // testing includes
    Course foundationsOfStatistics;
    foundationsOfStatistics.title = (char*)malloc(MAX_LENGTH * sizeof(char));
    strcpy(foundationsOfStatistics.title, "Foundations of Statistics");
    foundationsOfStatistics.period = 5;
    strcpy(foundationsOfStatistics.weekday, "Friday");

    printf("Title: %s, Weekday: %s, Period: %d\n", foundationsOfStatistics.title,
                                                   foundationsOfStatistics.weekday,
                                                   foundationsOfStatistics.period);

    free(foundationsOfStatistics.title);

    return 0;
}
