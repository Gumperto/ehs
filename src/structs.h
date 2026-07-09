#ifndef STRUCTS
#define STRUCTS

#define ALLOCATION_LIMIT 128

typedef enum {
    TWO = 2
}Tuple;

typedef enum {
    MONDAY = 0,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
}Weekdays;

typedef enum {
    QUARTER_ONE = 0,
    QUARTER_TWO = 1,
    SEMESTER_DURATION = 2,
    NUM_PERIODS = 6,
    NUM_WEEKDAYS = 6
}ScheduleConstants;

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
    DuoList meetings;
    
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
    int courseCountTotal;

    // Separate schedules for q1 and q2 so no collisions
    Course* schedule[SEMESTER_DURATION][NUM_PERIODS][NUM_WEEKDAYS];
    int courseCountTaken;

    // These two flags should probably be a placeholder
    // for now. Ideally we want the user to pick what periods
    // they don't want (1st period) and what days they don't 
    // want (Saturday, Monday, etc)
    int saturdayCheck;
    int period6Check;

    int totalCredits;
    int targetCredits;

    int allocationLimit;
}Schedule;

#endif
