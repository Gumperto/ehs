#include <stdbool.h>
#include "structs.h"

// ==============
// Adding courses
// ==============

bool isScheduleFull(Schedule* schedule) {
    if (schedule->courseArray[MAX_COURSES - 1] == NULL) return false;
    return true;
}

bool checkCourseEligibility(Schedule* schedule, Course* course) {
    if (schedule == NULL || course == NULL) return false;

    if (isScheduleFull(schedule) == true) return false;

    // initial filtering
    int takenCredit = course->takenCredit;
    if (takenCredit == 1) return false;

    for (int occurrenceIncrement = 0; occurrenceIncrement < course->meetings->size; occurrenceIncrement++) {
        // Initialize some check variables
        int checkPeriod = course->meetings->list[occurrenceIncrement].tuple[PERIOD];
        int checkWeekday = course->meetings->list[occurrenceIncrement].tuple[WEEKDAY];

        // Do not add the course if it overlaps with a course 
        // that already occupied at least one of its meetings.
        // (We assume the course added before is strictly better)
        for (int quarter = QUARTER_ONE; quarter < SEMESTER_DURATION; quarter++) {
            if (course->durationDuo.tuple[quarter] == 0) continue; // if course not held in some quarter, skip
            else {
                if (schedule->schedule[quarter][checkPeriod][checkWeekday] != NULL) 
                    return false;
            }
        }
    }
    return true;
}

bool addCourseToSchedule(Schedule* schedule, Course* course) {
    if (schedule == NULL || course == NULL) return false;
    if (checkCourseEligibility(schedule, course) == false) return false;

    for (int occurrenceIncrement = 0; occurrenceIncrement < course->meetings->size; occurrenceIncrement++) {
        for (int quarter = QUARTER_ONE; quarter < SEMESTER_DURATION; quarter++) {
            if (course->durationDuo.tuple[quarter] == 0) continue;

            int period = course->meetings->list[occurrenceIncrement].tuple[PERIOD];
            int weekday = course->meetings->list[occurrenceIncrement].tuple[WEEKDAY];
            schedule->schedule[quarter][period][weekday] = course;
        }
    }
    
    schedule->courseArray[schedule->courseCountTaken] = course;
    schedule->courseCountTaken++;
    if (course->isRequired == 1) schedule->requiredCourseCount++;
    schedule->totalCredits += course->credits;
    return true;
}
