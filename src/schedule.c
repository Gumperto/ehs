#include "structs.h"

void addRequired(Schedule sc, Course * class, int numCourses){
    //I did flag to ensure I don't add credits twice for whole semester courses
    int flag = 0;
    for(int i = 0; i < numCourses; i++){
        if((class + i) -> isRequired == 1 ){
            for(int j = 0; j < NUM_PERIODS; j++){
                for(int k = 0; k < NUM_WEEKDAYS; k++){
                    flag = 0;
                    if(class[i].q1_timeHeld[j][k] == 1){
                        sc.q1_courseList[j][k] = (class + i);
                        flag = 1;
                    }
                    if(class[i].q2_timeHeld[j][k] == 1){
                        sc.q2_courseList[j][k] = (class + i);
                        flag = 1;
                    }
                    if(flag == 1){
                        sc.totalCredits += (class+1)->credits;
                    }
                }
            }
        }
    }
}
