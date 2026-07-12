#include "debugCMD.h"
#include <stdio.h>
#include "structs.h"
#include "conversion.h"

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
    for(size_t courseIncrement = 0; courseIncrement < courseList->courseCountTotal; courseIncrement++) {
        Course* course = courseList->courseList[courseIncrement];
        printf("Course no.: %zu\n", courseIncrement);
        printCourseInformation(course);
        printf("\n");
    }
}

void printGeneralCourseListInfo(CourseList* courseList) {
    printf("Total courses in courseList: %zu\n", courseList->courseCountTotal);
    printf("Total required courses in courseList: %zu\n", courseList->requiredCourseTotal);
}

void printGeneralScheduleInformation(Schedule* schedule) {
    printf("Total courses taken: %zu\n", schedule->courseCountTaken);
    printf("Total credits of taken courses: %d\n", schedule->totalCredits);
    printf("Target credit of this semester: %d\n", schedule->targetCredits);
    printf("Total required courses taken: %zu\n", schedule->requiredCourseCount);
    printf("\n");
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
    printQuarterTable(schedule, QUARTER_ONE, "Quarter 1");
    printQuarterTable(schedule, QUARTER_TWO, "Quarter 2");
}

void printCourseListInSchedule(Schedule* schedule) {
    for (size_t course = 0; course < schedule->courseCountTaken; course++) {
        printf("Course %zu: '%s'\n", course, schedule->courseArray[course]->title);
    }
    printf("\n");
}
