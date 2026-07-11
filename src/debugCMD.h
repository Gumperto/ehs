#ifndef DEBUGCMD
#define DEBUGCMD

#include "structs.h"
void printFullCourseList(CourseList* courseList);
void printGeneralCourseListInfo(CourseList* courseList);
void printGeneralScheduleInformation(Schedule* schedule);
void printCourseSlotsMatrix(Schedule* schedule);
void printCourseListInSchedule(Schedule* schedule);

#endif
