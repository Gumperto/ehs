#include <stdio.h>
#include "structs.h"
#include "startCMD.h"

int starter(int argc, char** argv){
    CMDArgs args;
    // This is somewhat of an anti-pattern; we don't really want to modify args
    // if we can help it, but this is just so the program has a place to store it all
    // This design decision might be fixed later
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
        printf("You are running manual\n");
        runManual(argv[0], args);
    }
   
    return 0;
}
