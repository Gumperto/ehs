#include "debugCMD.h"
#include <stdio.h>
#include "structs.h"

void printGeneralScheduleInformation(Schedule* schedule) {
    printf("Total courses in course list: %d\n", schedule->courseCountTotal);
    printf("Total courses taken: %d\n", schedule->courseCountTaken);
    printf("Total credits of taken courses: %d\n", schedule->totalCredits);
    printf("Target credit of this semester: %d\n", schedule->targetCredits);
    printf("\n");
}

void printFullCourseList(Schedule* schedule) {
    for(int courseIncrement = 0; courseIncrement < schedule->courseCountTotal; courseIncrement++) {
        Course* course = schedule->courseList[courseIncrement];
        printf("Course no.: %d\n", courseIncrement);
        printf("Course title: %s\n", course->title);
        printf("Course credits: %d\n", course->credits);
        printf("Course required: %d\n", course->isRequired);
        printf("Course category: %s\n", course->category);

        printf("Course schedule:\n");
        DuoList occurrenceList = course->meetings;
        for (int occurrenceIncrement = 0; occurrenceIncrement < occurrenceList.size; occurrenceIncrement++) {
            printf("- Weekday: %d, Period: %d\n", occurrenceList.list[occurrenceIncrement].tuple[WEEKDAY],
                                                  occurrenceList.list[occurrenceIncrement].tuple[PERIOD]);
        }
        printf("\n");
    }
}

void printCourseSlotsMatrix(Schedule* schedule) {
    for (int periods = 0; periods < NUM_PERIODS; periods++) {
        printf("Period %d ", periods + 1);
        for (int weekdays = MONDAY; weekdays < NUM_WEEKDAYS; weekdays++) {
            for (int quarter = QUARTER_ONE; quarter < SEMESTER_DURATION; quarter++) {
                Course* printedCourse = schedule->schedule[quarter][periods][weekdays];
                if (weekdays == NUM_WEEKDAYS - 1 && quarter == QUARTER_TWO) {
                    if (printedCourse == NULL)
                        printf("| (Empty) |\n");
                    else
                        printf("| %s |\n", schedule->schedule[quarter][periods][weekdays]->title);
                }

                else {
                    if (printedCourse == NULL)
                        printf("| (Empty) |");
                    else
                        printf("| %s |", schedule->schedule[quarter][periods][weekdays]->title);
                }
            }
            if (weekdays != NUM_WEEKDAYS - 1)
                printf(" ~ ");
        }
    }
}
