#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void random_course_list(){
    FILE *fptr;
    srand(time(NULL));
    char first_word[10][30] = {"Introduction to ","Analysis of ","Help me ","Eating ","Advanced ","Intermediate ","History of ","Cirno's Perfect ","Basics of ","Critiques of "};
    char second_word[10][30] = {"Touhou","Dolphin","YagokoroEirin","Suisei","Pickle","Psyduck","Perfect Math Class","Kuril Islands","Madoka","Van Lang"};
    fptr = fopen("random_courses.txt","w");
    for(int i = 0; i < 100; i++){
        int first = rand()%10;
        int second = rand()%10;
        fprintf(fptr, "%s",first_word[first]);
        fprintf(fptr, "%s",second_word[second]);
        fprintf(fptr, ", ");
        int duration = rand()%3;
        if(duration == 0){
            fprintf(fptr, "Semester, ");
        }else if (duration == 1){
            fprintf(fptr, "Q1, ");
        }else{
            fprintf(fptr, "Q2, ");
        }
        int frequency = rand()%3 + 1;
        int periods_considered [frequency];
        for(int j = 0; j < frequency; j++){
            int is_same = 0;
            while(is_same == 0){
                periods_considered[j] = rand()%30;
                is_same = 1;
                for(int i = 0; i < j; i++){
                    if(periods_considered[j] == periods_considered[i]){
                        is_same = 0;
                    }
                }
            }
        }
        fprintf(fptr, "[");
        for(int k = 0; k < frequency; k++){
            int day = periods_considered[k]/6;
            if(day == 0){
                fprintf(fptr, "%s: ", "Mon");
            } else if (day == 1){
                fprintf(fptr, "%s: ", "Tue");
            } else if (day == 2){
                fprintf(fptr, "%s: ", "Wed");
            } else if (day == 3){
                fprintf(fptr, "%s: ", "Thu");
            } else if (day == 4){
                fprintf(fptr, "%s: ", "Sat");
            } else {
                fprintf(fptr, "%s: ", "Fri");
            }
            fprintf(fptr, "%d ", periods_considered[k]%6 + 1);
            if(k < (frequency-1)){
                fprintf(fptr, "; ");
            }
        }
        fprintf(fptr, "], ");

        int credits = rand()%5 + 1;

        int percentage_required  = rand()%10;

        if(percentage_required == 0){
            fprintf(fptr, " %d, %d, ", credits, 1);
        }else{
            fprintf(fptr, " %d, %d, ", credits, 0);
        }

        int cat = rand()%8;

        if(cat == 0){
            fprintf(fptr, " %s ","Humanities");
        }else if (cat == 1){
            fprintf(fptr, " %s ","Math");
        }else if (cat == 2){
            fprintf(fptr, " %s ","CSCE");
        }else if (cat == 3){
            fprintf(fptr, " %s ","VTubers");
        }else if (cat == 4){
            fprintf(fptr, " %s ","Social Sciences");
        }else if (cat == 5){
            fprintf(fptr, " %s ","Clowning");
        }else if (cat == 6){
            fprintf(fptr, " %s ","Global");
        }else {
            fprintf(fptr, " %s ","Japanese Language");
        }


        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

int main(void){
    random_course_list();
}
