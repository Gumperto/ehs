#include <stdlib.h>
#include "chooseCourse.h"
#include "structs.h"
#include "sortCoursesByCriteria.h"

void maximizeCreditsDumb(Schedule* schedule, CourseList* courseList) {
    // sort course list by credits highest -> lowest
    sortCourseList(courseList, "credits");

    size_t increment = 0;
    while((schedule->totalCredits) < (schedule->targetCredits) && increment < courseList->courseCountTotal){
        bool check = addCourseToSchedule(schedule, courseList->courseList[increment]);
        if (check == false) {
            increment++;
            continue;
        }
    }
}
