#include "debugCMD.h"
#include <stdio.h>
#include "structs.h"
#include "conversion.h"

// ================
// CourseList stuff
// ================

void printCourseInformation(Course* course) {
    printf("Course title: %s\n", course->title);
    printf("Course credits: %d\n", course->credits);
    printf("Course required: %d\n", course->isRequired);
    printf("Course category: %s\n", course->category);
    printf("Course duration: [%d, %d]\n", course->durationDuo.tuple[QUARTER_ONE],
                                          course->durationDuo.tuple[QUARTER_TWO]);

    printf("Course schedule:\n");
    for (int occurrenceIncrement = 0; occurrenceIncrement < course->meetings->size; occurrenceIncrement++) {
        int numeric_weekday = course->meetings->list[occurrenceIncrement].tuple[WEEKDAY];
        int numeric_period = course->meetings->list[occurrenceIncrement].tuple[PERIOD] + 1;
        printf("- Weekday: %s, Period: %d\n", stringNumberToWeek(numeric_weekday), numeric_period);
    }
}


void printFullCourseList(CourseList* courseList) {
    printf("== FULL CourseList ==\n");
    for(size_t courseIncrement = 0; courseIncrement < courseList->courseCountTotal; courseIncrement++) {
        Course* course = courseList->courseList[courseIncrement];
        printf("Course no.: %zu\n", courseIncrement);
        printCourseInformation(course);
        printf("\n");
    }
    printf("\n====================\n");
}

void printGeneralCourseListInfo(CourseList* courseList) {
    printf("== General CourseList info ==\n");
    printf("Total courses in courseList: %zu\n", courseList->courseCountTotal);
    printf("Total required courses in courseList: %zu\n", courseList->requiredCourseTotal);
    printf("\n=============================\n");
}

// ==============
// Schedule stuff
// ==============

void printGeneralScheduleInformation(Schedule* schedule) {
    printf("== General schedule information ==\n");
    printf("Total courses taken: %zu\n", schedule->courseCountTaken);
    printf("Total credits of taken courses: %d\n", schedule->totalCredits);
    printf("Total required courses taken: %zu\n", schedule->requiredCourseCount);
    printf("\n==================================\n");
}

void printQuarterTable(Schedule *schedule, int quarter, const char *label) {
    printf("=== %s ===\n", label);
    printf("%-10s", "");
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        printf("%-35s", stringNumberToWeek(weekday));
    printf("\n");

    for (int period = PERIOD_1; period < NUM_PERIODS; period++) {
        printf("Period %-3d", period + 1);
        for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++) {
            Course* course = schedule->schedule[quarter][period][weekday];
            if (course == NULL)
                printf("%-35s", "(Empty)");
            else
                printf("%-35s", course->title);
        }
        printf("\n");
    }
    printf("\n");
}

void printCourseSlotsMatrix(Schedule *schedule) {
    printf("== Course slots matrix ==\n");
    printQuarterTable(schedule, QUARTER_ONE, "Quarter 1");
    printQuarterTable(schedule, QUARTER_TWO, "Quarter 2");
    printf("\n=========================\n");
}

void printCourseListInSchedule(Schedule* schedule) {
    printf("== Course list in schedule ==\n");
    for (size_t course = 0; course < schedule->courseCountTaken; course++) {
        printf("Course %zu: '%s'\n", course, schedule->courseArray[course]->title);
    }
    printf("\n=============================\n");
}

// ============
// MasterChecks
// ============
void printMasterChecks(MasterCheck* mastercheck) {
    printf("== MasterCheck prints ==\n");
    
    printf("Target credits: %d\n", mastercheck->targetCredits);

    printf("Days that are okay: ");
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        if (mastercheck->weekdayCheck[weekday] == 1) printf("%s ", stringNumberToWeek(weekday));

    printf("\n");

    printf("Periods that are okay: ");
    for (int period = PERIOD_1; period < NUM_PERIODS; period++)
        if (mastercheck->periodCheck[period] == 1) printf("%d ", period + 1);

    printf("\n========================\n");
}
