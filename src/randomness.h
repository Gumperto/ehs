#ifndef RANDOMNESS
#define RANDOMNESS

#include "structs.h"
#include <stdbool.h>

int min(int a, int b);
int bernoulli(int denominator);
int randIntUniform(int range);
double probUniform();

void shuffleCourseList(Course** array, size_t size);
Schedule* buildRandomSchedule(CourseList* courseList);
bool isCourseInScheduleAlready(Schedule* schedule, Course* course);
bool addRandomCourseNotInSchedule(Schedule* schedule, const CourseList* courseList);

#endif
