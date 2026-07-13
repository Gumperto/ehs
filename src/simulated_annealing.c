// implementation inspired by https://github.com/MNoorFawi/simulated-annealing-in-c
// and the pseudocode from https://en.wikipedia.org/wiki/Simulated_annealing#Pseudocode

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "randomness.h"
#include "objective.h"
#include "structs.h"
#include "schedule.h"
#include "debugCMD.h"

#define MAX_ITERATIONS 10000
#define MIN_TEMP 0.001
#define MAX_TEMP 1000000

typedef enum {
    REMOVE = 0,
    ADD,
    SWAP
}PickNeighborAct;

Schedule* getNeighborSchedule(Schedule* schedule, CourseList* courseList) {
    Schedule* neighbor = deepCloneSchedule(schedule);
    if (neighbor == NULL) return NULL;
    
    int action = rand() % 3;

    // action is REMOVE
    // remove course from the neighbor schedule, like that's it
    if (action == REMOVE) {
        if (neighbor->courseCountTaken != 0) {
            int index = rand() % neighbor->courseCountTaken;
            scheduleRemoveCourseAt(neighbor, index);
        }
    }
    
    // action is ADD
    // adds a random course not in the neighbor schedule right now
    else if (action == ADD) {
        addRandomCourseNotInSchedule(neighbor, courseList);
    }

    // action is SWAP
    // all it does is delete a random course and then 
    // add a random course that is not in the schedule right now
    else if (action == SWAP) {
        if (neighbor->courseCountTaken != 0) {
            int index = rand() % neighbor->courseCountTaken;
            scheduleRemoveCourseAt(neighbor, index);
            addRandomCourseNotInSchedule(neighbor, courseList);
        }
    }

    return neighbor;
}

// based on https://en.wikipedia.org/wiki/Simulated_annealing#Acceptance_probabilities_2
double acceptanceProbability(double current_cost, double new_cost, double temperature) {
    if (new_cost < current_cost) return 1;
    else
        return exp(-(new_cost - current_cost)/temperature);
}

Schedule* simulatedAnnealing(CourseList* courseList, MasterCheck* mastercheck, 
                             double INIT_TEMP, double COOLDOWN, int verbose) {
    double temperature = INIT_TEMP;
    double p_accept;
    double current_cost, new_cost;

    char randomString[128];

    // build a random schedule
    Schedule* original = buildRandomSchedule(courseList);
    Schedule* current = original;

    // loop until temperature is cooled down totally
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        // copy schedule into neighbor
        Schedule* neighbor = getNeighborSchedule(current, courseList);
        if (neighbor == NULL) return NULL;

        // calculate cost of current solution and cost of new solution to compare
        current_cost = objective(current, courseList, mastercheck);
        new_cost = objective(neighbor, courseList, mastercheck);

        // a probability to help not looping forever
        p_accept = acceptanceProbability(current_cost, new_cost, temperature);

        // check if the probability function is larger than some probability between 0 and 1
        if (p_accept > probUniform()) {
            // if so then update neighbor to be the new solution
            freeSchedule(current);
            current = neighbor;
            current_cost = new_cost;
        }

        else
            freeSchedule(neighbor);

        // cool down
        temperature *= COOLDOWN;
        
        // This is so it doesn't cool down immediately
        if (temperature < MIN_TEMP) temperature = MIN_TEMP;
        // and this is to ensure it's not an inf bug when testing on edge cases
        if (temperature > MAX_TEMP) temperature = MAX_TEMP;
        
        if (verbose) {
            if (i % 100 == 0) {
                printf("\nCurrent cost is: %lf\n", new_cost);
                printf("Iteration: %d\n", i);
                printf("Temperature: %lf\n", temperature);
                printf("Cooldown: %lf\n", COOLDOWN);
            }
         }
    }
    
    printf("SA algorithm arrived at schedule with cost: %lf\n", current_cost);
    return current;
}

void simulatedAnnealing__wrapper(CourseList* courseList, MasterCheck* mastercheck, 
                                 double INIT_TEMP, double COOLDOWN, int verbose) {
    Schedule* schedule = simulatedAnnealing(courseList, mastercheck, INIT_TEMP, 
                                            COOLDOWN, verbose);
    printCourseSlotsMatrix(schedule);
    free(schedule);
}
