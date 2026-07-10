#ifndef SCHEDULE
#define SCHEDULE

#include "structs.h"
#include <stdbool.h>

void initCourse(Course* course);
Course* createCourse();
void destroyCourse(Course* course);

void initCourseList(CourseList* courseList);
CourseList* createCourseList();
bool pushCourseList(CourseList* courseList, Course* course);
void clearCourseList(CourseList* courseList);
void freeCourseList(CourseList* courseList);

void initSchedule(Schedule* schedule);
Schedule* createSchedule(int targetCredits, int saturdayCheck, int period6Check);
void clearSchedule(Schedule* schedule);
void freeSchedule(Schedule* schedule);

#endif
