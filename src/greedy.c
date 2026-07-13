#include <stdlib.h>
#include <stdio.h>
#include "chooseCourse.h"
#include "schedule.h"
#include "structs.h"
#include "objective.h"
#include "sortCoursesByCriteria.h"
#include "debugCMD.h"

// O(nlogn) where n is number of courses
Schedule* maximizeCreditsDumb(CourseList* courseList, MasterCheck* mastercheck) {
    Schedule* schedule = createSchedule();
    if (schedule == NULL) return NULL;

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
    double cost = objective(schedule, courseList, mastercheck);
    printf("Greedy (credits) algorithm arrived at schedule with cost: %lf\n", cost);
    return schedule;
}

void maximizeCreditsDumb__wrapper(CourseList* courseList, MasterCheck* mastercheck) {
    Schedule* schedule = maximizeCreditsDumb(courseList, mastercheck);
    printCourseSlotsMatrix(schedule);
    freeSchedule(schedule);
}
