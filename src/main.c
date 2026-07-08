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
    Course foundationsOfStatistics = {0};
    foundationsOfStatistics.title = (char*)malloc(MAX_LENGTH * sizeof(char));
    foundationsOfStatistics.category = (char*)malloc(MAX_LENGTH * sizeof(char));

    strcpy(foundationsOfStatistics.title, "Foundations of Statistics");
    strcpy(foundationsOfStatistics.category, "Math");
    foundationsOfStatistics.q1_timeHeld[4][4] = 1;
    foundationsOfStatistics.q2_timeHeld[4][4] = 1;
    foundationsOfStatistics.credits = 2;
    foundationsOfStatistics.isRequired = 1;
    printf("Title: %s\n", foundationsOfStatistics.title);

    free(foundationsOfStatistics.title);

    return 0;
}
