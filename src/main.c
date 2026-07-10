// IMPORTANT: please keep main.c as small as possible;
// everything here is just placeholder as a proof of 
// concept at the time being (I do mean EVERYTHING)
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "readfile.h"
#include "debugCMD.h"
#include "schedule.h"

#define MAX_LENGTH 128

int main() {
    CourseList* courseList = createCourseList();
    Schedule* schedule = createSchedule(20, 0, 0);
    
    char filename[MAX_LENGTH];
    printf("What's the file name: ");
    scanf("%s", filename);
    
    readfile(courseList, filename);

    printFullCourseList(courseList);
    printCourseSlotsMatrix(schedule);

    freeCourseList(courseList);

    return 0;
}
