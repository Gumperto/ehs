/*#include <stdio.h>
#include "structs.h"

#define MAX_LENGTH 128

void starter(Schedule sc){
    printf("Welcome to the East Hokusai Course Registration Optimizer!\n");
    //call addRequired() here once we have an acutal course list
    // addRequired(sc);
    printf("We have added your required courses, which add up to %d credits. \n", sc.totalCredits);
    printf("How many credits would you like to get this semester? ");
    scanf("%d",&(sc.targetCredits));

    printf("Would you like to take courses on Saturday? Default = n [y/n]:");
    char saturdayConfirmation;
    scanf("%c",&saturdayConfirmation);
    //we should change this to handle errors later
    if(saturdayConfirmation == 'y' || saturdayConfirmation == 'Y'){
        sc.saturday = 1;
    }else {
        sc.saturday = 0;
    }
    printf("Would you like to take courses on Period 6? Default = n [y/n]");
    char p6_confirmation;
    scanf("%c",&p6_confirmation);
    //we should change this to handle errors later
    if(p6_confirmation == 'y'){
        sc.period6 = 1;
    }else {
        sc.period6 = 0;
    }
    while(sc.totalCredits < sc.targetCredits){
        printf("What kind of courses would you like me to add?");
        char preferred[MAX_LENGTH];
        scanf("%s", preferred);
        int number_to_add = 0;
        printf("How many of those kind of courses should I add? ");
        scanf("%d",&number_to_add);
        for(int i = 0; i < number_to_add; i++){
            //add that kind of courses (We haven't created the functionality)
        }
    }
}
*/
