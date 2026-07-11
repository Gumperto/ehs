#ifndef CHOOSECOURSE
#define CHOOSECOURSE

#include "structs.h"
#include <stdbool.h>

bool checkCourseEligibility(Schedule* schedule, Course* course);
bool addCourseToSchedule(Schedule* schedule, Course* course);

#endif
