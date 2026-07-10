#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int comp_credits(const void * a, const void * b){
    printf("a is: %s \n",((Course *)a)->title);
    printf("b is: %s \n",((Course *)b)->title);
    printf("Address of a is: %p \n",a);
    printf("Address of b is: %p \n",b);
    return(((*(Course **)b)->credits)-((*(Course **)a)->credits));
}

// void addRequired(Schedule sc, Course * class, int numCourses){
//     //I did flag to ensure I don't add credits twice for whole semester courses
//     int flag = 0;
//     for(int i = 0; i < numCourses; i++){
//         if((class + i) -> isRequired == 1 ){
//             for(int j = 0; j < NUM_PERIODS; j++){
//                 for(int k = 0; k < NUM_WEEKDAYS; k++){
//                     flag = 0;
//                     if(class[i].q1_timeHeld[j][k] == 1){
//                         sc.q1_courseList[j][k] = (class + i);
//                         flag = 1;
//                     }
//                     if(class[i].q2_timeHeld[j][k] == 1){
//                         sc.q2_courseList[j][k] = (class + i);
//                         flag = 1;
//                     }
//                     if(flag == 1){
//                         sc.totalCredits += (class+1)->credits;
//                     }
//                 }
//             }
//         }
//     }
// }





//void requiredCreditGreedy(Schedule* schedule) {
    // addRequired();
    // then implement credit greedy on the rest
    // return
//}

