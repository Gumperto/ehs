#ifndef CHOOSECOURSE
#define CHOOSECOURSE

#include "structs.h"
#include <stdbool.h>

bool checkCourseEligibility(Schedule* schedule, Course* course);
bool addCourse(Schedule* schedule, Course* course);

#endif
