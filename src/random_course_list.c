#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void random_course_list(){
    FILE *fptr;
    srand(time(NULL));
    char first_word[10][30] = {"Introduction to ","Analysis of ","Help me ","Eating ","Advanced ","Intermediate ","History of ","Fries with ","Basics of ","Critiques of "};
    char second_word[10][30] = {"Touhou","Dolphin","えーりん","星街すいせい","Pickle","コダック","Perfect Math Class","Kuril Islands","Madoka","Van Lang"};
    fptr = fopen("random_courses.txt","w");
    for(int i = 0; i < 100; i++){
        int first = rand()%10;
        int second = rand()%10;
        fprintf(fptr, "%s",first_word[first]);
        fprintf(fptr, "%s",second_word[second]);
        fprintf(fptr, ", ");
        int duration = rand()%3;
        if(duration == 0){
            fprintf(fptr, "Semester,");
        }else if (duration == 1){
            fprintf(fptr, "Q1,");
        }else{
            fprintf(fptr, "Q2,");
        }
        int frequency = rand()%3;
        for(int j = 0; j < (frequency+1); j++){
            int weekday = rand()%7;
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}
