#ifndef STRUCTS
#define STRUCTS

#include <stddef.h>

#define RANDOM_COURSE_NUMBER 512
#define MAX_LENGTH 256
#define MAX_TRIALS_RANDOM 1000

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

typedef enum {
    ERROR = -1,
    HELP = 0,
    AUTO = 1,
    MANUAL = 2
}RunOptions;

typedef enum {
    ALGO_COUNT = 2,
    GREEDY_ALG = 0,
    SIMAN_ALG = 1
}AlgoCode;

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

    // 1 for required, 0 for not required
    int isRequired;
 
    // 1 for have taken in the past, 0 for not taken in the past (unused)
    int takenCredit;

    char* category;
}Course;

typedef struct {
    // Stores the list of course pointers in memory
    Course** courseList;
    size_t courseCountTotal;
    size_t capacity;
    size_t requiredCourseTotal;
}CourseList;

typedef struct {
    // 1 on an index means allowed, 0 means not allowed (Monday = 0)
    int weekdayCheck[NUM_WEEKDAYS];

    // 1 on an index means allowed, 0 means not allowed (Period 1 = 0)
    int periodCheck[NUM_PERIODS];

    // How many credits are specified by the user?
    int targetCredits;    
}MasterCheck;

typedef struct {
    // Separate schedules for q1 and q2 so no collisions
    Course* schedule[SEMESTER_DURATION][NUM_PERIODS][NUM_WEEKDAYS];
    size_t courseCountTaken;
    size_t requiredCourseCount;

    // List of courses chosen for this schedule (not CourseList* because that owns the courses)
    Course* courseArray[MAX_COURSES];

    // How many credits have already been taken by this schedule arrangement?
    int totalCredits;
}Schedule;

typedef struct {
    char fileName[MAX_LENGTH];
    char algorithm[MAX_LENGTH];
    int algorithmCode;

    int pickedAlgorithmFlag;
    int targetCreditsFlag;
    int targetCredits;
    int weekdayCheck[NUM_WEEKDAYS];
    int weekdayFlag;
    int periodCheck[NUM_PERIODS];
    int periodFlag;

    int cooldownSetFlag;
    double cooldown;
    int initTempSetFlag;
    double initTemp;

    int randomFlag;
    int nameFlag;
    int helpFlag;
    int errFlag;

    int randomSeedFlag;
    int randomSeed;
    int verbose;
}CMDArgs;

#endif
