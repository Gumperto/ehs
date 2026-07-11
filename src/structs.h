#ifndef STRUCTS
#define STRUCTS

#include <stddef.h>

typedef enum {
    TWO = 2
}Tuple;

typedef enum {
    QUARTER_ONE = 0,
    QUARTER_TWO = 1,
    SEMESTER_DURATION = 2,
    NUM_PERIODS = 6,
    NUM_WEEKDAYS = 6,
    MAX_COURSES = (SEMESTER_DURATION * NUM_PERIODS * NUM_WEEKDAYS)
}ScheduleConstants;

typedef enum {
    PERIOD = 0,
    WEEKDAY
}OccurrenceIndex;

typedef struct {
    int tuple[TWO];
}Duo;

typedef struct {
    Duo* list;
    int size;
}DuoList;

typedef struct {
    char* title;

    // array of Duo's, showing what weekday and period the class takes place in
    DuoList* meetings;

    // duo that tracks the semester duration of a course
    Duo durationDuo;
    
    int credits;

    //1 for required, 0 for not required
    int isRequired;
 
    //1 for have taken in the past, 0 for not taken in the past (unused)
    int takenCredit;

    char* category;
}Course;

typedef struct {
    // Stores the list of course pointers in memory
    Course** courseList;
    size_t courseCountTotal;
    size_t capacity;
}CourseList;

typedef struct {
    // Separate schedules for q1 and q2 so no collisions
    Course* schedule[SEMESTER_DURATION][NUM_PERIODS][NUM_WEEKDAYS];
    size_t courseCountTaken;

    // 1 on an index means allowed, 0 means not allowed (Monday = 0)
    int weekdayCheck[NUM_WEEKDAYS];

    // 1 on an index means allowed, 0 means not allowed (Period 1 = 0)
    int periodCheck[NUM_PERIODS];

    // List of courses chosen for this schedule (not CourseList* because that owns the courses)
    Course* courseArray[MAX_COURSES];

    // How many credits have already been taken by this schedule arrangement?
    int totalCredits;
    int targetCredits;
}Schedule;

#endif
