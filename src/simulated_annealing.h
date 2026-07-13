#ifndef SIM_AN
#define SIM_AN

#include "structs.h"
Schedule* simulatedAnnealing(CourseList* courseList, MasterCheck* mastercheck, 
                             double INIT_TEMP, double COOLDOWN, int k_max, int verbose);
void simulatedAnnealing__wrapper(CourseList* courseList, MasterCheck* mastercheck,
                                 double INIT_TEMP, double COOLDOWN, int k_max, int verbose,
                                 int seed, const Hyperparams hyperparams);

#endif
