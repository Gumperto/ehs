#ifndef SCHEDULE
#define SCHEDULE

#include "structs.h"
#include <stdbool.h>

void initDuoList(DuoList* list);
DuoList* createDuoList();
void destroyDuoList(DuoList* list);

void initCourse(Course* course);
Course* createCourse();
void destroyCourse(Course* course);
Course* setCourse(char* title, char* duration, char* meetings, int credit, int required, char* category);

void initCourseList(CourseList* courseList);
CourseList* createCourseList();
bool pushCourseList(CourseList* courseList, Course* course);
bool clearCourseList(CourseList* courseList);
void freeCourseList(CourseList* courseList);

void initSchedule(Schedule* schedule);
Schedule* createSchedule();
Schedule* deepCloneSchedule(Schedule* schedule);
bool scheduleRemoveCourseAt(Schedule* schedule, int index);
bool clearSchedule(Schedule* schedule);
void freeSchedule(Schedule* schedule);

void initMasterCheck(MasterCheck* mastercheck);
MasterCheck* createMasterCheck();
void fillMasterCheck(MasterCheck* mastercheck, int targetCredits, const int* weekdayCheck, const int* periodCheck);
bool clearMasterCheck(MasterCheck* mastercheck);
void freeMasterCheck(MasterCheck* mastercheck);

#endif
