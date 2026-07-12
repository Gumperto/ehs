// For probabilistic stuff and some math

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"
#include "structs.h"
#include "chooseCourse.h"

int min(int a, int b) {
    if (a > b) return b;
    else return a;
}

// Uniform number from 0 to range - 1
int randIntUniform(int range) {
    int r, rand_max = RAND_MAX - (RAND_MAX % range);
    while ((r = rand()) >= rand_max);
    return r / (rand_max / range);
}

// uniform between 0 and 1
double probUniform() {
    return (double)rand() / (double)RAND_MAX ;
}

// produces p = 1/denominator bernoulli dist; P(1) = p, P(0) = 1 - p
int bernoulli(int denominator) {
    if (rand() % denominator == 0) {
        return 1;
    }
    else return 0;
}

/* shuffles a COPY of the course list's array
* size is just the size of the original array */
void shuffleCourseList(Course** array, size_t size) {
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = size - 1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);
        Course* tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

bool isCourseInScheduleAlready(Schedule* schedule, Course* course) {
    for (size_t i = 0; i < schedule->courseCountTaken; i++) {
        if (course == schedule->courseArray[i])
            return true;
    }
    return false;
}

// picks out a candidate course randomly and adds it to a schedule
bool addRandomCourseNotInSchedule(Schedule* schedule, const CourseList* courseList) {
    // create a temp, writable array of course pointers
    Course** temp = (Course**)malloc(courseList->courseCountTotal * sizeof(Course*));
    memcpy(temp, courseList->courseList, courseList->courseCountTotal * sizeof(Course*));
    shuffleCourseList(temp, courseList->courseCountTotal);

    for (int i = 0; i < min(courseList->courseCountTotal, MAX_TRIALS_RANDOM); i++) {
        Course* course = temp[i];
        if (isCourseInScheduleAlready(schedule, course)) continue;
        else {
            if(addCourseToSchedule(schedule, course) == true) {
                free(temp);
                return true;
            }
            else continue;
        }
    }
    free(temp);
    return false;
}

Schedule* buildRandomSchedule(const CourseList* courseList) {
    Schedule* schedule = createSchedule();

     // create a temp, writable array of course pointers
    Course** temp = (Course**)malloc(courseList->courseCountTotal * sizeof(Course*));
    memcpy(temp, courseList->courseList, courseList->courseCountTotal * sizeof(Course*));
    shuffleCourseList(temp, courseList->courseCountTotal);

    for (size_t i = 0; i < courseList->courseCountTotal; i++) {
        addCourseToSchedule(schedule, temp[i]);
    }

    free(temp);
    return schedule;
}
