#include <stdlib.h>
#include "chooseCourse.h"
#include "structs.h"
#include "sortCoursesByCriteria.h"

void maximizeCreditsDumb(Schedule* schedule, CourseList* courseList, MasterCheck* mastercheck) {
    // sort course list by credits highest -> lowest
    sortCourseList(courseList, "credits");

    size_t increment = 0;
    while((schedule->totalCredits) < (mastercheck->targetCredits) && increment < courseList->courseCountTotal){
        bool check = addCourseToSchedule(schedule, courseList->courseList[increment]);
        if (check == false) {
            increment++;
            continue;
        }
    }
}
