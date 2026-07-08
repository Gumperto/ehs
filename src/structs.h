#ifndef STRUCTS
#define STRUCTS

/* HELP: if you feel like there's any other data field
* that would be helpful to add (i.e bool mandatory, char*
* lecturer, bool required, etc) feel free to add it below */
typedef struct {
    char* title;
    char weekday[16];
    int period;
}Course;

/* HELP: what do we actually want the Schedule struct 
* to do? like what benefit does it offer over just an
* array of Course (i.e Course*)                    */
typedef struct {
    Course * courseList[6][6];
    
}Schedule;

#endif
