#ifndef STRUCTS
#define STRUCTS

enum {
    SEMESTER_DURATION = 2,
    NUM_PERIODS = 6,
    NUM_WEEKDAYS = 6
};

/* HELP: if you feel like there's any other data field
* that would be helpful to add (i.e bool mandatory, char*
* lecturer, bool required, etc) feel free to add it below */
typedef struct {
    char* title;
    //[1,1] for full semester, [1,0] for first half only, [0,1] for summer only
    int whichQuarter[SEMESTER_DURATION];
    
    //the matrix is 0 when there's no class and 1 when there is class: 
    //if there's class at only Mon Period 3 for full semester, only q1_timeheld[0][2] = 1
    //                                                         and  q2_timeheld[0][2] = 1
    //                                                         rest of matrix         = 0
    int q1_timeHeld[NUM_PERIODS][NUM_WEEKDAYS];
    int q2_timeHeld[NUM_PERIODS][NUM_WEEKDAYS];
    
    int credits;

    //1 for required, 0 for not required
    int isRequired;
    
    char* category;
}Course;

/* HELP: what do we actually want the Schedule struct 
* to do? like what benefit does it offer over just an
* array of Course (i.e Course*)                    */
typedef struct {
    Course* q1_courseList[NUM_PERIODS][NUM_WEEKDAYS];
    Course* q2_courseList[NUM_PERIODS][NUM_WEEKDAYS];

    // These two flags should probably be a placeholder
    // for now. Ideally we want the user to pick what periods
    // they don't want (1st period) and what days they don't 
    // want (Saturday, Monday, etc)
    int saturday;
    int period6;

    int totalCredits;
    int targetCredits;
}Schedule;

#endif
