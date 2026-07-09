#ifndef DEBUGCMD
#define DEBUGCMD

typedef enum {
    PERIOD = 0,
    WEEKDAY
}OccurrenceIndex;

#include "structs.h"
void printGeneralScheduleInformation(Schedule* schedule);
void printFullCourseList(Schedule* schedule);
void printCourseSlotsMatrix(Schedule* schedule);

#endif
