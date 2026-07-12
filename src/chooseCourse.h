#ifndef SCHEDULE
#define SCHEDULE

#include "structs.h"
void requiredCreditGreedy(Schedule* schedule);

int comp_credits(const void * a, const void * b);
void maximizeCredits(Schedule sc);
#endif
