#include <stdbool.h>
#include "structs.h"

// ==============
// Adding courses
// ==============

bool checkCourseEligibility(Schedule* schedule, Course* course) {
    if (schedule == NULL || course == NULL) return false;

    // initial filtering
    int takenCredit = course->takenCredit;
    if (takenCredit == 1) return false;

    for (int occurrenceIncrement = 0; occurrenceIncrement < course->meetings->size; occurrenceIncrement++) {
        // Initialize some check variables
        int checkPeriod = course->meetings->list[occurrenceIncrement].tuple[PERIOD];
        int checkWeekday = course->meetings->list[occurrenceIncrement].tuple[WEEKDAY];

        // Do not add the course if it's NOT required
        // **AND** falls on a bad day or bad period
        if ((schedule->periodCheck[checkPeriod] == 0 && course->isRequired == 0) ||
            (schedule->weekdayCheck[checkWeekday] == 0 && course->isRequired == 0))
            return false;

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

        // not added
        // if (schedule->isFull == 1) return false;
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
    schedule->totalCredits += course->credits;
    return true;
}

// void addRequired(Schedule sc, Course * class, int numCourses){
//     //I did flag to ensure I don't add credits twice for whole semester courses
//     int flag = 0;
//     for(int i = 0; i < numCourses; i++){
//         if((class + i) -> isRequired == 1 ){
//             for(int j = 0; j < NUM_PERIODS; j++){
//                 for(int k = 0; k < NUM_WEEKDAYS; k++){
//                     flag = 0;
//                     if(class[i].q1_timeHeld[j][k] == 1){
//                         sc.q1_courseList[j][k] = (class + i);
//                         flag = 1;
//                     }
//                     if(class[i].q2_timeHeld[j][k] == 1){
//                         sc.q2_courseList[j][k] = (class + i);
//                         flag = 1;
//                     }
//                     if(flag == 1){
//                         sc.totalCredits += (class+1)->credits;
//                     }
//                 }
//             }
//         }
//     }
// }
