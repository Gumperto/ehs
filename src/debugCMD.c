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

void printGeneralScheduleInformation(Schedule* schedule) {
    printf("Total courses taken: %zu\n", schedule->courseCountTaken);
    printf("Total credits of taken courses: %d\n", schedule->totalCredits);
    printf("Target credit of this semester: %d\n", schedule->targetCredits);
    printf("\n");
}

void printFullCourseList(CourseList* courseList) {
    for(size_t courseIncrement = 0; courseIncrement < courseList->courseCountTotal; courseIncrement++) {
        Course* course = courseList->courseList[courseIncrement];
        printf("Course no.: %zu\n", courseIncrement);
        printCourseInformation(course);
        printf("\n");
    }
}

void printCourseSlotsMatrix(Schedule* schedule) {
    for (int periods = PERIOD_1; periods < NUM_PERIODS; periods++) {
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
