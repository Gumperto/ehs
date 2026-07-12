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

bool destroyDuoList(DuoList* list) {
    if (list == NULL) return false;
    free(list->list);
    free(list);
    return true;
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

bool destroyCourse(Course* course) {
    if (course == NULL) return false;
    if (course->meetings != NULL) destroyDuoList(course->meetings);
    free(course->title);
    free(course->category);
    free(course);
    return true;
}

// For CourseList
void initCourseList(CourseList* courseList) {
    courseList->courseList = NULL;
    courseList->courseCountTotal = 0;
    courseList->capacity = 0;
    courseList->requiredCourseTotal = 0;
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
    if (course->isRequired == 1) courseList->requiredCourseTotal++;
    return true;
}

bool clearCourseList(CourseList* courseList) {
    if (courseList == NULL) return false;
    for (size_t i = 0; i < courseList->courseCountTotal; i++) {
        destroyCourse(courseList->courseList[i]);
    }
    free(courseList->courseList);
    initCourseList(courseList);
    return true;
}

void freeCourseList(CourseList* courseList) {
    if (clearCourseList(courseList) == false) return;
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
    schedule->requiredCourseCount = 0;

    // Initialize totalCredits taken
    schedule->totalCredits = 0;
}

Schedule* createSchedule() {
    Schedule* schedule = (Schedule*)malloc(sizeof(Schedule));
    if (schedule == NULL) return NULL;
    initSchedule(schedule);
    return schedule;
}

Schedule* deepCloneSchedule(Schedule* schedule) {
    Schedule* scheduleNew = createSchedule();

    scheduleNew->courseCountTaken = schedule->courseCountTaken;
    scheduleNew->requiredCourseCount = schedule->requiredCourseCount;
    scheduleNew->totalCredits = schedule->totalCredits;
    
    // Copy all slots
    for (int quarter = QUARTER_ONE; quarter < SEMESTER_DURATION; quarter++) {
        for (int period = PERIOD_1; period < NUM_PERIODS; period++) {
            for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++) {
                scheduleNew->schedule[quarter][period][weekday] = schedule->schedule[quarter][period][weekday];
            }
        }
    }

    // Copy all members in internal course list
    for (size_t course = 0; course < schedule->courseCountTaken; course++)
        scheduleNew->courseArray[course] = schedule->courseArray[course];

    return scheduleNew;
}

bool clearSchedule(Schedule* schedule) {
    if (schedule == NULL) return false;
    initSchedule(schedule);
    return true;
}

void freeSchedule(Schedule* schedule) {
    if(clearSchedule(schedule) == false) return;
    free(schedule);
}

// For MasterCheck
void initMasterCheck(MasterCheck* mastercheck) {
    // Initialize all days being available
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        mastercheck->weekdayCheck[weekday] = 1;

    // Initialize all periods being available
    for (int period = 0; period < NUM_PERIODS; period++)
        mastercheck->periodCheck[period] = 1;

    mastercheck->targetCredits = 0;
}

MasterCheck* createMasterCheck() {
    MasterCheck* mastercheck = (MasterCheck*)malloc(sizeof(MasterCheck));
    if(mastercheck == NULL) return NULL;
    initMasterCheck(mastercheck);
    return mastercheck;
}

void fillMasterCheck(MasterCheck* mastercheck, int targetCredits, int* weekdayCheck, int* periodCheck) {
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        mastercheck->weekdayCheck[weekday] = weekdayCheck[weekday];

    for (int period = 0; period < NUM_PERIODS; period++)
        mastercheck->periodCheck[period] = periodCheck[period];

    mastercheck->targetCredits = targetCredits;
}

bool clearMasterCheck(MasterCheck* mastercheck) {
    if (mastercheck == NULL) return false;
    initMasterCheck(mastercheck);
    return true;
}

void freeMasterCheck(MasterCheck* mastercheck) {
    if(clearMasterCheck(mastercheck) == false) return;
    free(mastercheck);
}
