#include <string.h>
#include <stdio.h>
#include "structs.h"

void addResultToFile(const int algoCode, const int seed, const Hyperparams hyperparams, 
                     const double finalScore, const double timeForRun, const CMDArgs args) {
    char outputName[MAX_LENGTH];
    int writeHeader = 0;

    if (args.outputFlag == 1) strcpy(outputName, args.outputName);
    else strcpy(outputName, "results.csv");

    // opens in read
    FILE* checkptr = fopen(outputName,"r");

    // file DNE
    if (checkptr == NULL) writeHeader = 1;
    // file empty
    else {
        int c = fgetc(checkptr);
        if (c == EOF) writeHeader = 1;
        else ungetc(c, checkptr);
        fclose(checkptr);
    }

    if (algoCode == SIMAN_ALG) {
        FILE* fptr = fopen(outputName,"a");
        if (fptr == NULL) {
            fprintf(stderr, "Could not open '%s' to write\n", outputName);
            return;    
        }

        if (writeHeader == 1)
            fprintf(fptr, "algorithm,seed,init_temp_siman,cooldown_siman,k_max,finalScore,runtime\n");

        fprintf(fptr, "%s,%d,%.6lf,%.6lf,%d,%.2lf,%.3lf\n",
                "simulated_annealing",  
                seed, 
                hyperparams.init_temp_siman, 
                hyperparams.cooldown_siman, 
                hyperparams.k_max, 
                finalScore, 
                timeForRun);
        printf("Appended results to '%s'\n", outputName);
        fclose(fptr);
    }

    else if (algoCode == GREEDY_ALG) {
        FILE* fptr = fopen(outputName,"a");
        if (fptr == NULL) {
            fprintf(stderr, "Could not open '%s' to write\n", outputName);
            return;    
        }

        if (writeHeader == 1)
            fprintf(fptr, "algorithm,seed,finalScore,runtime\n");

        fprintf(fptr, "%s,%d,%.2lf,%.3lf",
                "greedy", 
                seed, 
                finalScore, 
                timeForRun);
        fclose(fptr);
    }

    // ...
    // add more algos here
    // ...

    else {
        fprintf(stderr, "Genuinely don't know how this would happen. Report this bug to https://github.com/Gumperto/ehs.\n");
        return;
    }
}
