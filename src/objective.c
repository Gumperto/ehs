#include <limits.h>
#include "structs.h"
#include "chooseCourse.h"

/* Scores a specific schedule based on a few criteria (lower is better):*
* - Starts from 0
* - If no courses or credits are taken at all: immediate worst score
* - For each gap between two courses on the same day: somewhat bad penalty for each gap, increasing linearly
* - For each day going to school: quite bad penalty for each day, increasing linearly 
* - Under/over credit: plus/minus 2 away from desired credit is low penalty, higher penalty as you move away
* - Class in a user-designated bad period: decently bad penalty, scales o(superlinearly) 
*                                          with amount of classes in bad periods 
* - Not taking all required credits: not sure about this. The isRequired flag means required for graduation
*                                    not necessarily for the current semester. Maybe it should ask user how
*                                    many required credits they feel like they need to take? And then do
*                                    something similar to under/over credit. Feels a bit tired from a novelty POV
* - Others I haven't considered: feel free to suggest                                                          */
double objectiveFunction(Schedule* schedule) {
    int score = 0;
    return score;
}

