// IMPORTANT: please keep main.c as small as possible;
// everything here is just placeholder as a proof of 
// concept at the time being (I do mean EVERYTHING)
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "readfile.h"
#include "debugCMD.h"

#define MAX_LENGTH 128

int main() {
    Schedule* schedule = (Schedule*)malloc(sizeof(Schedule));
    schedule->allocationLimit = ALLOCATION_LIMIT;
    schedule->courseCountTotal = 0;
    schedule->courseCountTaken = 0;
    schedule->totalCredits = 0;
    schedule->targetCredits = 2000;

    schedule->saturdayCheck = 0;
    schedule->period6Check = 0;
    
    char filename[MAX_LENGTH];
    printf("What's the file name: ");
    scanf("%s", filename);
    
    readfile(schedule, filename);

    printGeneralScheduleInformation(schedule);
    printFullCourseList(schedule);

    return 0;
}
