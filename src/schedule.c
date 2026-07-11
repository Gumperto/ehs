#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"
#include "conversion.h"

#define INITIAL_ALLOCATION_LIMIT 128

// For DuoList

void initDuoList(DuoList* list) {
    list->list = NULL;
    list->size = 0;
}

DuoList* createDuoList() {
    DuoList* list = (DuoList*)malloc(sizeof(DuoList));
    initDuoList(list);
    return list;
}

void destroyDuoList(DuoList* list) {
    if (list == NULL) return;
    free(list->list);
    free(list);
}

// For Course
void initCourse(Course* course) {
    course->title = NULL;

    // DuoList
    course->meetings = NULL;

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
    if (course->meetings != NULL) destroyDuoList(course->meetings);
    free(course->title);
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
    if (course == NULL) return false;
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
    // Initialize all slots to NULL
    for (int quarter = QUARTER_ONE; quarter < SEMESTER_DURATION; quarter++) {
        for (int period = PERIOD_1; period < NUM_PERIODS; period++) {
            for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++) {
                schedule->schedule[quarter][period][weekday] = NULL;
            }
        }
    }

    // Initialize all members of the course array to NULL
    for (int course = 0; course < MAX_COURSES; course++)
        schedule->courseArray[course] = NULL;

    // Initialize number of courses taken this semester
    schedule->courseCountTaken = 0;

    // Initialize all days being available
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        schedule->weekdayCheck[weekday] = 1;

    // Initialize all periods being available
    for (int period = 0; period < NUM_PERIODS; period++)
        schedule->periodCheck[period] = 1;

    // Initialize totalCredits taken and targetCredits to take
    schedule->totalCredits = 0;
}

Schedule* createSchedule(int targetCredits, int* weekdayCheck, int* periodCheck) {
    Schedule* schedule = (Schedule*)malloc(sizeof(Schedule));
    if (schedule == NULL) return NULL;

    initSchedule(schedule);

    // Initialize necessary checks
    schedule->targetCredits = targetCredits;
    
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++) {
        schedule->weekdayCheck[weekday] = weekdayCheck[weekday];
    }

    for (int period = 0; period < NUM_PERIODS; period++) {
        schedule->periodCheck[period] = periodCheck[period];
    }

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
