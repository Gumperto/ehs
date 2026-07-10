#include "structs.h"
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_ALLOCATION_LIMIT 128

// For Course
void initCourse(Course* course) {
    course->title = NULL;

    // DuoList
    course->meetings.list = NULL;
    course->meetings.size = 0;

    // The rest
    course->credits = 0;
    course->isRequired = 0;
    course->takenCredit = 0;

    course->category = NULL;
}

Course* createCourse() {
    Course* course = (Course*)malloc(sizeof(Course));
    if (course == NULL) return NULL;
    initCourse(course);
    return course;
}

void destroyCourse(Course* course) {
    if (course == NULL) return;
    free(course->title);
    // bit of an antipattern
    free(course->meetings.list);
    free(course->category);
    free(course);
}

// For CourseList
void initCourseList(CourseList* courseList) {
    courseList->courseList = NULL;
    courseList->courseCountTotal = 0;
    courseList->capacity = 0;
}

CourseList* createCourseList() {
    CourseList* courseList = (CourseList*)malloc(sizeof(CourseList));
    if (courseList == NULL) return NULL;
    initCourseList(courseList);
    return courseList;
}

bool pushCourseList(CourseList* courseList, Course* course) {
    if (courseList->courseCountTotal == courseList->capacity) {
        if (courseList->capacity == 0) courseList->capacity = 4;
        size_t new_limit = courseList->capacity * 2;
        Course **tmp = realloc(courseList->courseList, new_limit * sizeof(Course *));
        if (!tmp) return false;
        courseList->courseList = tmp;
        courseList->capacity = new_limit;
    }
    courseList->courseList[courseList->courseCountTotal] = course;
    courseList->courseCountTotal++;
    return true;
}

void clearCourseList(CourseList* courseList) {
    if (courseList == NULL) return;
    for (size_t i = 0; i < courseList->courseCountTotal; i++) {
        destroyCourse(courseList->courseList[i]);
    }
    free(courseList->courseList);
    initCourseList(courseList);
}

void freeCourseList(CourseList* courseList) {
    clearCourseList(courseList);
    free(courseList);
}

// For schedule
void initSchedule(Schedule* schedule) {
    // Initialize everything to NULL
    for (int quarter = 0; quarter < SEMESTER_DURATION; quarter++) {
        for (int period = 0; period < NUM_PERIODS; period++) {
            for (int weekday = 0; weekday < NUM_WEEKDAYS; weekday++) {
                schedule->schedule[quarter][period][weekday] = NULL;
            }
        }
    }

    // Initialize number of courses taken this semester
    schedule->courseCountTaken = 0;

    // Initialize totalCredits taken and targetCredits to take
    schedule->totalCredits = 0;
}

Schedule* createSchedule(int targetCredits, int saturdayCheck, int period6Check) {
    Schedule* schedule = (Schedule*)malloc(sizeof(Schedule));
    if (schedule == NULL) return NULL;

    initSchedule(schedule);

    // Initialize necessary checks
    schedule->targetCredits = targetCredits;
    schedule->saturdayCheck = saturdayCheck;
    schedule->period6Check = period6Check;
    
    return schedule;
}

void clearSchedule(Schedule* schedule) {
    if (schedule == NULL) return;
    initSchedule(schedule);
}

void freeSchedule(Schedule* schedule) {
    clearSchedule(schedule);
    free(schedule);
}
