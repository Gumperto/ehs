#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "structs.h"
#include "conversion.h"

#define ASTRONOMICAL_PENALTY 1000000

#define EXTRENELY_BAD_PENALTY 1000
#define REALLY_BAD_PENALTY 500
#define BAD_PENALTY 300
#define MILD_PENALTY 100

#define POINTLESS_PERIOD_COUNT 1
#define BAD_PERIOD_COUNT 3
#define EXPONENTIAL_BASE_BURNOUT 10

double gapPenalty(Schedule* schedule) {
    double penalty = 0;
    for (int q = QUARTER_ONE; q < SEMESTER_DURATION; q++) {
        for (int wd = MONDAY; wd < NUM_WEEKDAYS; wd++) {
            int firstInstance = -1;
            for (int p = PERIOD_1; p < NUM_PERIODS; p++) {
                // Initially move both pointers to the first non-empty p (firstInstance)
                if (schedule->schedule[q][p][wd] == NULL) continue;
                else {
                    firstInstance = p;
                    break;
                }
            }
            if (firstInstance == -1) continue;

            // Freeze pointer 1 at firstInstance
            int index1 = firstInstance;
            // Move pointer 2 always; if encounter a non-empty course
            // calculate distance to pointer 1 and teleport it to 2
            int index2 = firstInstance;
            
            while ((index2 += 1) < NUM_PERIODS) {
                if (schedule->schedule[q][index2][wd] == NULL) continue;
                else {
                    int gap = index2 - index1 - 1; // -1 is gap-correction
                    if (gap <= 2) penalty += gap * BAD_PENALTY; 
                    else if (gap == 3) penalty += gap * REALLY_BAD_PENALTY;
                    else if (gap >= 4) penalty += gap * EXTRENELY_BAD_PENALTY;
                    index1 = index2;
                }
            }
        }
    }
    printf("gapPenalty: %lf\n", penalty);
    return penalty;
}

double commutePenalty(Schedule* schedule) {
    double penalty = 0;
    for (int q = QUARTER_ONE; q < SEMESTER_DURATION; q++) {
        for (int wd = MONDAY; wd < NUM_WEEKDAYS; wd++) {
            for (int p = PERIOD_1; p < NUM_PERIODS; p++) {
                if (schedule->schedule[q][p][wd] == NULL) continue;
                else {
                    penalty += REALLY_BAD_PENALTY;
                    break;
                }
            }
        }
    }
    printf("commutePenalty: %lf\n", penalty);
    return penalty;
}

double creditDivergencePenalty(Schedule* schedule, MasterCheck* mastercheck) {
    double penalty = 0;
    int distance = abs(schedule->totalCredits - mastercheck->targetCredits);
    penalty += distance * distance * BAD_PENALTY;
    printf("creditDivergencePenalty: %lf\n", penalty);
    return penalty;
}

double badSlotPenalty(Schedule* schedule, MasterCheck* mastercheck) {
    double penalty = 0;
    for (int q = QUARTER_ONE; q < SEMESTER_DURATION; q++) {
        int badSlotCount = 0;
        for (int p = PERIOD_1; p < NUM_PERIODS; p++) {
            for (int wd = MONDAY; wd < NUM_WEEKDAYS; wd++) {
                if (mastercheck->periodCheck[p] == 1 && mastercheck->weekdayCheck[wd] == 1) continue;
                if (schedule->schedule[q][p][wd] != NULL) badSlotCount++;
            }
        }
        penalty += REALLY_BAD_PENALTY * badSlotCount * badSlotCount;
    }
    printf("badSlotPenalty: %lf\n", penalty);
    return penalty;
}

double requirementPenalty(Schedule* schedule, CourseList* courseList) {
    double penalty = 0;

    // division by 8 to roughly estimate 8 semesters = 4 years * 2 semesters/year of college
    int distance = (int)courseList->requiredCourseTotal / 8 - (int)schedule->requiredCourseCount; 
    if (distance < 0) penalty = 0;
    else penalty = BAD_PENALTY * distance * distance;

    printf("requirementPenalty: %lf\n", penalty);

    return penalty;
}

double courseCountPenalty(Schedule* schedule) {
    double penalty = 0;
    for (int q = QUARTER_ONE; q < SEMESTER_DURATION; q++) {
        for (int wd = MONDAY; wd < NUM_WEEKDAYS; wd++) {
            int periodCount = 0;
            for (int p = PERIOD_1; p < NUM_PERIODS; p++) {
                if (schedule->schedule[q][p][wd] != NULL)
                    periodCount++;
            }
                if (periodCount == POINTLESS_PERIOD_COUNT) penalty += BAD_PENALTY;
                else if (periodCount > BAD_PERIOD_COUNT) penalty += pow(EXPONENTIAL_BASE_BURNOUT, periodCount - BAD_PERIOD_COUNT) * BAD_PENALTY;
        }
    }
    printf("courseCountPenalty: %lf\n", penalty);
    return penalty;
}

/* Scores a specific schedule based on a few criteria (lower is better):*
* - Starts from 0
* - If no courses or credits are taken at all: immediate worst score
* - For each gap between two courses on the same day: somewhat bad penalty for each gap, increasing linearly
* - For each day going to school: quite bad penalty for each day, increasing linearly 
* - Under/over credit: plus/minus 2 away from desired credit is low penalty, higher penalty as you move away
* - Class in a user-designated bad period: very bad penalty, scales o(superlinearly) 
*                                          with amount of classes in bad periods 
* - Not taking all required credits: quadratic penalty if lacking, no penalty else. estimates the number of 
*                                    required courses aimed for in a semester over 4 years is total number of 
*                                    required courses / 8. This difference is squared and then multiplied with BAD_PENALTY
* - 1 slot taken in a day or >3 taken in a day: only 1 slot taken: BAD_PENALTY
*                                               >3 slot taken: exponential penalty for each course over the 3rd, I *know* how terrible this is */
double objective(Schedule* schedule, CourseList* courseList, MasterCheck* mastercheck) {
    double score = 0;
    if (schedule->courseCountTaken == 0 || schedule->totalCredits == 0)
         score += ASTRONOMICAL_PENALTY;

    score += gapPenalty(schedule);
    score += commutePenalty(schedule);
    score += creditDivergencePenalty(schedule, mastercheck);
    score += badSlotPenalty(schedule, mastercheck);
    score += requirementPenalty(schedule, courseList);
    score += courseCountPenalty(schedule);

    return score;
}

