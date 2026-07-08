#ifndef STRUCTS
#define STRUCTS

/* HELP: if you feel like there's any other data field
* that would be helpful to add (i.e bool mandatory, char*
* lecturer, bool required, etc) feel free to add it below */
typedef struct {
    char* title;
    //[1,1] for full semester, [1,0] for first half only, [0,1] for summer only
    int runduration [2];
    //the matrix is 0 when there's no class and 1 when there is class: 
    //if there's class at only Mon Period 3 only q1_timeheld[0][2] and q2_timeheld[0][2] will be 1, rest of matrix will be 0
    int q1_timeheld[6][6];
    int q2_timeheld[6][6];
    int credits;
    //1 for required, 0 for not required
    int isRequired;
    char category[20];
}Course;

/* HELP: what do we actually want the Schedule struct 
* to do? like what benefit does it offer over just an
* array of Course (i.e Course*)                    */
typedef struct {
    Course * q1_courseList[6][6];
    Course * q2_courseList[6][6];
}Schedule;


void addRequired(Schedule sc, Course * class, int numCourses){
    for(int i = 0; i < numCourses; i++){
        if((class + i) -> isRequired == 1 ){
            for(int j = 0; j < 6; j++){
                for(int k = 0; k < 6; k++){
                    if(class[i].q1_timeheld[j][k] == 1){
                        sc.q1_courseList[j][k] = (class + i);
                    }
                    if(class[i].q2_timeheld[j][k] == 1){
                        sc.q2_courseList[j][k] = (class + i);
                    }
                }
            }
        }
    }
}
#endif
