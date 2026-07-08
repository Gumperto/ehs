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
    int saturday;
    int period6;
    int credits;
    int target_credits;
}Schedule;


//we should work on this thing next, all I did was open and close the file, should this be part of main?
void readfile(char * coursesTXT){
    FILE *fptr;
    fptr = fopen(coursesTXT,"r");
    if(fptr == NULL){
        printf("Error File not found\n");
        return 0;
    }
    fclose(fptr);
}


void addRequired(Schedule sc, Course * class, int numCourses){
    //I did flag to ensure I don't add credits twice for whole semester courses
    int flag = 0;
    for(int i = 0; i < numCourses; i++){
        if((class + i) -> isRequired == 1 ){
            for(int j = 0; j < 6; j++){
                for(int k = 0; k < 6; k++){
                    flag = 0;
                    if(class[i].q1_timeheld[j][k] == 1){
                        sc.q1_courseList[j][k] = (class + i);
                        flag = 1;
                    }
                    if(class[i].q2_timeheld[j][k] == 1){
                        sc.q2_courseList[j][k] = (class + i);
                        flag = 1;
                    }
                    if(flag == 1){
                        sc.credits += (class+1)->credits;
                    }
                }
            }
        }
    }
}

void starter(Schedule sc){
    printf("Welcome to the East Hokusai Course Registration Optimizer\n");
    //call addRequired() here once we have an acutal course list
    printf("We have added your required courses, which add up to %d credits. \n", sc.credits);
    printf("How many credits would you like to get this semester? ");
    scanf("%d",&(sc.target_credits));
    printf("Would you like to take courses on Saturday? (y/n)");
    char thing;
    scanf("%c",&thing);
    //we should change this to handle errors later
    if(thing == 'y'){
        sc.saturday = 1;
    }else if (thing == 'n'){
        sc.saturday = 0;
    }
    printf("Would you like to take courses on Period 6? (y/n)");
    char thing;
    scanf("%c",&thing);
    //we should change this to handle errors later
    if(thing == 'y'){
        sc.period6 = 1;
    }else if (thing == 'n'){
        sc.period6 = 0;
    }
    while(sc.credits < sc.target_credits){
    printf("What kind of courses would you like me to add?");
    char preferred[50];
    scanf("%s",&preferred);
    int number_to_add = 0;
    printf("How many of those kind of courses should I add? ");
    scanf("%d",&number_to_add);
    for(int i = 0; i < number_to_add; i++){
        //add that kind of courses (We haven't created the functionality)
    }
    }
}
#endif

