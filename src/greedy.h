#ifndef GREEDY
#define GREEDY

#include "structs.h"
Schedule* maximizeCreditsDumb(CourseList* courseList, MasterCheck* mastercheck);
void maximizeCreditsDumb__wrapper(CourseList* courseList, MasterCheck* mastercheck, 
                                  int seed, const Hyperparams hyperparams, CMDArgs args);

#endif
