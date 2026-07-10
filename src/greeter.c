#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "schedule.h"
#include "readfile.h"
#include "debugCMD.h"
#include "conversion.h"

#define MAX_LENGTH 256

void fetchFilename(char* filename) {
    printf("You are inputting filename manually.\n");
    printf("Pro-tip: you can use cmdline arguments with ehs!\n");
    printf("For example, type \n");
    printf("$ ehs {filename}\n");
    printf("to automatically use that argument as the filename!\n");
    printf("Input filename manually: ");
    scanf("%s", filename);
}

void resolveAvailabilityChecks(int* weekdayCheck, int* periodCheck) {
    char temp[MAX_LENGTH];
    char delimiter[] = " ";

    // Read into periodCheck
    printf("Input all periods you do not want to attend, separated by spaces: ");
    fgets(temp, MAX_LENGTH, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    
    int increment = 0;
    char* token = strtok(temp, delimiter);
    while(token != NULL && increment < NUM_PERIODS) {
        int numeric_period = atoi(token) - 1; // the -1 is index correction
        printf("Getting rid of period '%s'\n", token);
        if (numeric_period >= NUM_PERIODS || numeric_period < PERIOD_1) {
            printf("Failure!\n");
            token = strtok(NULL, delimiter);
            continue;
        }
        printf("Success!\n");
        periodCheck[numeric_period] = 0;
        increment++; // add only after a valid check
        token = strtok(NULL, delimiter);
    }

    // Read into weekdayCheck
    printf("Input all weekdays you do not want to attend, separated by spaces: ");
    fgets(temp, MAX_LENGTH, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    
    increment = 0;
    token = strtok(temp, delimiter);
    while(token != NULL && increment < NUM_WEEKDAYS) {
        int numeric_weekday = stringWeekToNumber(token);
        printf("Getting rid of weekday '%s'\n", token);
        if (numeric_weekday < MONDAY || numeric_weekday >= NUM_WEEKDAYS) {
            printf("Failure!\n");
            token = strtok(NULL, delimiter);
            continue;
        }
        printf("Success!\n");
        weekdayCheck[numeric_weekday] = 0;
        increment++;
        token = strtok(NULL, delimiter);
    }

    printf("Days remaining: ");
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        if (weekdayCheck[weekday] == 1) printf("%s ", stringNumberToWeek(weekday));

    printf("\n");

    printf("Periods remaining: ");
    for (int period = PERIOD_1; period < NUM_PERIODS; period++)
        if (periodCheck[period] == 1) printf("%d ", period + 1);

    printf("\n");
}

int starter(int argc, char** argv){
    char filename[MAX_LENGTH];
    int targetCredits;
    int weekdayCheck[NUM_WEEKDAYS] = {1, 1, 1, 1, 1, 1};
    int periodCheck[NUM_PERIODS] = {1, 1, 1, 1, 1, 1};

    if (argc == 1) {
        fetchFilename(filename);
    }
    else {
        strcpy(filename, argv[1]);
    }

    CourseList* courseList = createCourseList();
    if (courseList == NULL) return 1;

    readfile(courseList, filename);

    resolveAvailabilityChecks(weekdayCheck, periodCheck);
    printf("How many credits are you aiming for this semester: ");
    scanf("%d", &targetCredits);

    Schedule* schedule = createSchedule(targetCredits, weekdayCheck, periodCheck);
    if (courseList == NULL) return 1;

    // addCourses();

    freeCourseList(courseList);
    freeSchedule(schedule);
    
    return 0;
}
