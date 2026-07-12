#include <stdio.h>
#include "structs.h"
#include "startCMD.h"

int starter(int argc, char** argv){
    CMDArgs args;
    int config = configureRun(argc, argv, &args);

    if (config == ERROR) {
        printf("There has been an error. Exiting...\n");
        return 1;
    }
    if (config == HELP) {
        printf("You are in help mode\n");
        return 0;
    }
    if (config == AUTO) {
        printf("You are running automatic\n");
        runAuto(args);
    }
    if (config == MANUAL) {
        runManual(argv[0], args);
        printf("You are running automatic\n");
    }
   
    return 0;
}
