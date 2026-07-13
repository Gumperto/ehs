#include <stdio.h>
#include "structs.h"

void addResultToFile(const int algoCode, const int seed, const Hyperparams hyperparams, 
                     const double finalScore, const double timeForRun) {
    // opens in append
    if (algoCode == SIMAN_ALG) {
        FILE* fptr = fopen("results_siman.csv","a");
        if (fptr == NULL) {
            fprintf(stderr, "Could not open file\n");
            return;    
        }

        fprintf(fptr, "%s,%d,%.6lf,%.6lf,%d,%.2lf,%.3lf\n",
                "simulated_annealing",  
                seed, 
                hyperparams.init_temp_siman, 
                hyperparams.cooldown_siman, 
                hyperparams.k_max, 
                finalScore, 
                timeForRun);
        fclose(fptr);
    }

    else if (algoCode == GREEDY_ALG) {
        FILE* fptr = fopen("results_greedy.csv","a");
        if (fptr == NULL) {
            fprintf(stderr, "Could not open file\n");
            return;    
        }

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
