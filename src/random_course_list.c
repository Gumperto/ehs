#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "randomness.h"

void random_course_list(int samples, char* random_file_name){
    FILE *fptr;
    char first_word[][MAX_LENGTH] = {
        "Introduction to ",
        "Analysis of ",
        "Help me ",
        "Eating ",
        "Advanced ",
        "Intermediate ",
        "History of ",
        "Cirno's Perfect ",
        "Basics of ",
        "Critiques of "
    };

    char second_word[][MAX_LENGTH] = {
        "Touhou",
        "Dolphin",
        "Eirin",
        "Suisei",
        "Pickle",
        "Psyduck",
        "Perfect Math Class",
        "Kuril Islands",
        "Madoka",
        "Van Lang", 
        "ArchBtw"
    };

    fptr = fopen(random_file_name,"w");

    int requiredSoFar = 0;
    int requiredCap = min(samples, RANDOM_COURSE_NUMBER);
    if (requiredCap == RANDOM_COURSE_NUMBER) requiredCap /= 5;

    for(int i = 0; i < samples; i++){
        // Title
        int first = rand()%10;
        int second = rand()%10;

        fprintf(fptr, "%s", first_word[first]);
        fprintf(fptr, "%s", second_word[second]);
        fprintf(fptr, ", ");

        // Duration
        int duration = rand()%3;

        if(duration == 0){
            fprintf(fptr, "Semester, ");
        }else if (duration == 1){
            fprintf(fptr, "Q1, ");
        }else{
            fprintf(fptr, "Q2, ");
        }

        // Frequency
        int frequency = rand()%3 + 1;
        fprintf(fptr, "%d, ", frequency);

        int periods_considered[frequency];

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

        // Credits
        int credits = rand()%4 + 1;
        // Required at a 1/5 probability
        int required = 0;
        if (requiredCap > requiredSoFar) {
            requiredSoFar++;
            required = bernoulli(5);
        }
        fprintf(fptr, " %d, %d, ", credits, required);

        // Category
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
