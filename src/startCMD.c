#include <math.h>
#ifdef __unix__
    #include <unistd.h>
    #include <getopt.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "randomness.h"
#include "structs.h"
#include "conversion.h"
#include "stringOps.h"
#include "schedule.h"
#include "readfile.h"
#include "greedy.h"
#include "random_course_list.h"
#include "simulated_annealing.h"

#define DEFAULT_TEMP_SIMAN 5000
#define DEFAULT_COOLDOWN_SIMAN 0.995
#define DEFAULT_K_MAX 10000

// does not set default name; if not supplied, exit
int fetchFilename(char* executablePath, char* filename) {
    int check;

    printf("You are inputting filename manually.\n");
    printf("Pro-tip: you can use cmdline arguments with ehs!\n");
    printf("For example, type \n");
    printf("\t%s {filename}\n", executablePath);
    printf("to automatically use that argument as the filename!\n");

    #ifdef __unix__
        printf("\nTo see more other commandline stuff, try:\n");
        printf("\t%s --help\n\n", executablePath);
    
    #else
        printf("You can also randomly generate a file and use that by trying:\n");
        printf("\t%s {filename} random\n\n", argv[0]);
    #endif

    printf("Input filename manually: ");

    // Actually scan the filename
    fgets(filename, MAX_LENGTH - 1, stdin);
    check = sscanf(filename, "%s", filename);
    if (check != 1) {
        fprintf(stderr, "Invalid or empty input. Exiting...\n");
        return 0;
    }
    
    printf("\n");

    return 1;
}

// sets all to 1 by default
void resolveAvailabilityChecks(int* weekdayCheck, int* periodCheck, const CMDArgs args) {
    char temp[MAX_LENGTH];
    char delimiter[] = " ";

    // Read into periodCheck
    if (args.periodFlag == 0) {
        printf("Input all periods you do not want to attend, separated by commas ',' (e.g 1,2,3): ");
        fgets(temp, MAX_LENGTH, stdin);
        temp[strcspn(temp, "\n")] = '\0';
        
        parsePeriodList(temp, periodCheck);
    }

    printf("\n");

    // Read into weekdayCheck
    if (args.weekdayFlag == 0) {
        printf("Input all weekdays you do not want to attend, separated by commas ',' (e.g Mon,Sat): ");
        fgets(temp, MAX_LENGTH, stdin);
        temp[strcspn(temp, "\n")] = '\0';
        
        parseWeekdayList(temp, weekdayCheck);
    }

    printf("\n");
}

// returns a bad estimate by default based on courseList
int fetchTargetCredits(const CourseList* courseList) {
    int targetCredits = -1;
    char buffer[MAX_LENGTH];
    printf("How many credits are you aiming for this semester (leaving this empty defaults to a VERY bad estimate): ");
    
    // Actually scan the target credits
    fgets(buffer, MAX_LENGTH - 1, stdin);
    int check = sscanf(buffer, "%d", &targetCredits);

    // If no input was given or if the input sucks
    if (check != 1 || targetCredits <= 0) {
        printf("Invalid or empty input. Calculating default value...\n");
        int courseCap = min(courseList->courseCountTotal, RANDOM_COURSE_NUMBER);
        int totalCreditsInCourseList = 0;
        for (int course = 0; course < courseCap; course++)
            totalCreditsInCourseList += courseList->courseList[course]->credits;

        // 8 semesters
        targetCredits = totalCreditsInCourseList / 8;

        fprintf(stderr, "Using very rough estimate: %d\n\n", targetCredits);
    }
    
    printf("\n");

    return targetCredits;
}

// returns SIMAN_ALG by default
int fetchAlgorithm() {
    int algoCode;

    char algo_list[][MAX_LENGTH] = {"greedy", "simulated_annealing"};
    char buffer[MAX_LENGTH];
    printf("What kind of algorithm do you want to use?\n"
           "What we have to offer (leaving empty uses %s by default): ", algo_list[SIMAN_ALG]);

    for (int i = GREEDY_ALG; i < ALGO_COUNT; i++) {
        printf("%s", algo_list[i]);
        if (i != ALGO_COUNT - 1) printf(", ");
    }

    printf("\n\nInput (exact match): ");

    // Actually scan the algorithm
    fgets(buffer, MAX_LENGTH - 1, stdin);
    int check = sscanf(buffer, "%s", buffer);

    // If no input was given or if the input sucks
    if (check != 1) {
        fprintf(stderr, "Invalid or empty input; using simulated annealing by default\n\n");
        algoCode = SIMAN_ALG;
        return algoCode;
    }

    if (strcmp(buffer, algo_list[GREEDY_ALG]) == 0)
        algoCode = GREEDY_ALG;

    else if (strcmp(buffer, algo_list[SIMAN_ALG]) == 0 || 
             strcmp(buffer, "siman") == 0 ||  
             strcmp(buffer, "simulated_anneal") == 0 ||
             strcmp(buffer, "sim_anneal") == 0)
        algoCode = SIMAN_ALG;

    else {
        fprintf(stderr, "Invalid or empty input; using simulated annealing by default\n\n");
        algoCode = SIMAN_ALG;
    }

    printf("\n");

    return algoCode;
}

// fetches a generic double safely with default
// upper and lower bounds are exclusive (not inclusive)
double fetchGenericDoubleWithDefault(const char* stdprmpt, const char* var_name, const char* purpose,
                                     const char* stderror, double defaultVal, 
                                     double upperbound, double lowerbound) {
    double returnVal;
    char buffer[MAX_LENGTH];

    printf("%s %s for %s: ", stdprmpt, var_name, purpose);
    
    fgets(buffer, MAX_LENGTH - 1, stdin);
    int check = sscanf(buffer, "%lf", &returnVal);

    // If no input was given or if the input sucks
    if (check != 1 || returnVal > upperbound || returnVal < lowerbound) {
        fprintf(stderr, "%s; using %lf by default\n", stderror, defaultVal);
        returnVal = defaultVal;
    }

    return returnVal;
}

// default depends on the algorithm
// also fetches from args if they're already set
Hyperparams fetchHyperparams(const int algoCode, const CMDArgs args) {
    Hyperparams hyper = {0};
    char stdprmpt[] = "Input";
    char stderror[] = "Invalid or empty input";

    if (args.pickedAlgorithmFlag == 1) {
        if (algoCode == SIMAN_ALG) {
            hyper.init_temp_siman = args.initTemp;
            hyper.cooldown_siman = args.cooldown;
            hyper.k_max = args.k_max;
            return hyper;
        }

        // ...
        // add more if more
        // ...

        else
            return hyper;
    }

    else {
        if (algoCode == SIMAN_ALG) {
            char purpose[] = "simulated_annealing";
            hyper.init_temp_siman = fetchGenericDoubleWithDefault(stdprmpt, 
                                                                  "init_temp_siman", 
                                                                  purpose, 
                                                                  stderror, 
                                                                  DEFAULT_TEMP_SIMAN,
                                                                  INFINITY,
                                                                  0);
            hyper.cooldown_siman = fetchGenericDoubleWithDefault(stdprmpt, 
                                                                 "cooldown_siman", 
                                                                 purpose, 
                                                                 stderror, 
                                                                 DEFAULT_COOLDOWN_SIMAN,
                                                                 INFINITY,
                                                                 0);
            hyper.k_max = (int)fetchGenericDoubleWithDefault(stdprmpt,
                                                             "k_max",
                                                             purpose,
                                                             stderror,
                                                             DEFAULT_K_MAX,
                                                             INT_MAX,
                                                             0);
            printf("\n");
            return hyper;
        }
        
        // ...
        // add more if more
        // ...

        else {
            printf("\n");
            return hyper;
        }
    }
}

CMDArgs cmdlineGetArgs(int argc, char** argv) {
    CMDArgs args = {0};
    args.randomSeed = 42;
    for (int i = MONDAY; i < NUM_WEEKDAYS; i++) args.weekdayCheck[i] = 1;
    for (int i = PERIOD_1; i < NUM_PERIODS; i++) args.periodCheck[i] = 1;

    #ifdef __unix__
        while (1) {
            int option_index = 0;
            static struct option long_options[] = 
            {
                {"random-gen",     optional_argument, NULL,  'r'},
                {"data",           required_argument, NULL,  'd'},
                {"output",         required_argument, NULL,  'o'},
                {"seed",           required_argument, NULL,  's'},
                {"help",           no_argument,       NULL,  'h'},
                {"verbose",        no_argument,       NULL,  'v'},
                {"algorithm",      required_argument, NULL,  'a'},
                {"target-credits", required_argument, NULL,  't'},
                {"weekday-bad",    required_argument, NULL,  'w'},
                {"period-bad",     required_argument, NULL,  'p'},
                {"k-max",          required_argument, NULL,  'k'},
                {"cooldown",       required_argument, NULL,  'c'},
                {"temp",           required_argument, NULL,  'T'},
                {NULL,             0,                 NULL,    0}
            };

            // the really scary string below is just how getopt_long parses stuff
            // - in front: parses optarg in while loop
            // : in front of everything: disables auto error so we do it
            // each letter represents a short command (can correspond with a long, as seen above)
            // :: after a letter means optional arg
            // : after a letter means required arg
            // just the character means no arguments needed
            int c = getopt_long(argc, argv, "-:rd:hva:t:w:p:c:T:s:k:o:", long_options, &option_index);
            if (c == -1)
              break;

            switch (c) {
                case 0:
                    printf("long option %s", long_options[option_index].name);
                    if (optarg)
                       printf(" with arg %s", optarg);
                    printf("\n");
                    break;

                // regular argument
                case 1:
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    strcpy(args.fileName, optarg);
                    args.nameFlag = 1;
                    break;

                case 'h':
                    printf("Usage:\t");
                    printf("$ %s [--help] [--data=path] [--random_generated=seed]\n",
                            argv[0]);
                    printf("GUIDE:\n");
                    printf("  %s\n      %s\n", "--help, -h", "print this menu");
                    printf("  %s\n      %s\n", "--verbose, -v", "print detailed execution results");
                    printf("  %s\n      %s\n", "--data=[path], -d [path]", "required; tells ehs what file to read");
                    printf("  %s\n      %s\n", "--output=[name], -o [name]", "required; tells ehs what to output to");
                    printf("  %s\n      %s\n", "--random-gen, -r", "uses randomly generated data; you may set seed with --seed");
                    printf("  %s\n      %s\n", "--seed=[seed], -s [seed]", "required; seeds the random processes in the program with a number");
                    printf("  %s\n      %s\n", "--algorithm=[algo], -a [algo]", "required; choose out of [greedy | simulated_annealing]");
                    printf("      %s\n", "note: simulated_annealing also requires --cooldown, --temp, and --k-max");
                    printf("  %s\n      %s\n", "--target-credits=[credits], -t [credits]", "required; specifies target credits");
                    printf("  %s\n      %s\n", "--weekday-bad=[list], -w [list]", "comma-separated bad weekdays, e.g. Mon,Sat (as in type it out like this)");
                    printf("  %s\n      %s\n", "--period-bad=[list], -p [list]", "comma-separated bad periods, e.g. 1,2 (as in type it out like this)");
                    printf("  %s\n      %s\n", "--k-max=[iter], -k [iter]", "required; number of k iterations for simulated annealing");
                    printf("  %s\n      %s\n", "--cooldown=[rate], -c [rate]", "required; cooldown rate for simulated annealing");
                    printf("  %s\n      %s\n", "--temp=[temp], -T [temp]", "required; initial temperature for simulated annealing");
                    
                    args.helpFlag = 1;
                    break;

                case 'r':
                    args.randomFlag = 1;
                    break;

                case 'd':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    strcpy(args.fileName, optarg);
                    args.nameFlag = 1;
                    break;

                case 'o':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    strcpy(args.outputName, optarg);
                    args.outputFlag = 1;
                    break;

                case 's':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }

                    char *endptr;
                    long seed = strtol(optarg, &endptr, 10);
                    if (*endptr != '\0' || seed < 0) {
                        fprintf(stderr, "Random seed must be a non-negative integer\n");
                        args.errFlag = 1;
                        break;
                    }
                    args.randomSeed = (int)seed;
                    args.randomSeedFlag = 1;
                    break;

                case 'v':
                    args.verbose = 1;
                    break;

                case 'a':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    strcpy(args.algorithm, optarg);
                    args.pickedAlgorithmFlag = 1;
                    break;

                case 't':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    args.targetCredits = atoi(optarg);
                    if (args.targetCredits < 0) {
                        fprintf(stderr, "Target credits is >= 0! Exiting...\n");
                        args.errFlag = 1;
                    }
                    args.targetCreditsFlag = 1;
                    break;

                case 'w':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    args.weekdayFlag = 1;
                    parseWeekdayList(optarg, args.weekdayCheck);
                    break;

                case 'p':
                    if (optarg == NULL) {
                        args.errFlag = 1;    
                        break;
                    }
                    args.periodFlag = 1;
                    parsePeriodList(optarg, args.periodCheck);
                    break;
                
                case 'k':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    args.k_maxSetFlag = 1;
                    args.k_max = atoi(optarg);
                    if (args.k_max <= 0) {
                        fprintf(stderr, "k_max is > 0! Exiting...\n");
                        args.errFlag = 1;
                    }
                    break;

                case 'c':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    args.cooldownSetFlag = 1;
                    args.cooldown = atof(optarg);
                    if (args.cooldown < 0) {
                        fprintf(stderr, "Cooldown is >= 0! Exiting...\n");
                        args.errFlag = 1;
                    }
                    break;

                case 'T':
                    if (optarg == NULL) {
                        args.errFlag = 1;
                        break;
                    }
                    args.initTempSetFlag = 1;
                    args.initTemp = atof(optarg);
                    if (args.initTemp < 0) {
                        fprintf(stderr, "Init temp is >= 0! Exiting...\n");
                        args.errFlag = 1;
                    }
                    break;

                case '?':
                    printf("Unknown option '%c'\n", optopt);
                    printf("Try:\n");
                    printf("%s --help\n", argv[0]);
                    printf("to see all options\n");
                    args.errFlag = 1;
                    break;

                case ':':
                    printf("Missing option for '%c'\n", optopt);
                    printf("Try:\n");
                    printf("%s --help\n", argv[0]);
                    printf("to see all options\n");
                    args.errFlag = 1;
                    break;

                default:
                    printf("?? getopt returned character code 0%o ??\n", c);
                    args.errFlag = 1;
                    break;
             }
        }

        return args;
        
        #else
            if (argc == 1) {
                // fetchFilename modifies the argument directly
                args.nameFlag = 0;
                return args;
            }
            else if (argc == 2) {
                if (argv[1] == NULL) {
                    args.errFlag = 1;
                    return args;
                }
                strcpy(args.fileName, argv[1]);
                args.nameFlag = 1;
                return args;
            }
            
            else {
                if (argv[1] == NULL) {
                    args.errFlag = 1;
                    return args;
                }
                strcpy(args.fileName, argv[1]);
                args.nameFlag = 1;

                if (strncmp(argv[2], "random", 4) == 0)
                    args.randomFlag = 1;
                
                return args;
            }
            
    #endif

    fprintf(stderr, "This shouldn't happen! Report this bug to github.com/Gumperto/ehs if it does!\n");
    args.errFlag = 1;
    return args;
}

// the ONLY function in the entire program allowed to mess with args
int configureRun(int argc, char** argv, CMDArgs* args) {
    *args = cmdlineGetArgs(argc, argv);

    // Default 42
    if (args->randomSeedFlag == 1) srand(args->randomSeed);
    else {
        args->randomSeed = (int)time(NULL);
        srand(args->randomSeed);
    }

    // First check if any error occurred
    if (args->errFlag == 1) return ERROR;

    if (args->helpFlag == 1) return HELP;

    if (args->randomFlag == 1 && args->nameFlag == 1) {
        fprintf(
            stderr,
            "WARNING: randomFlag and nameFlag are both on. We will disable randomFlag and keep nameFlag on.\n"
        );
        args->randomFlag = 0;
    }

    // Check if it's an automatic run;
    // An automatic run is when:
    // - The file is random or supplied (would still run if it's an end, but check warning above)
    // - An algorithm is picked and necessary hyperparams are applied
    // - Target credit supplied
    // - Weekdays & Periods all supplied
    bool isAuto = ( (args->randomFlag == 1 || args->nameFlag == 1) &&
                    (args->pickedAlgorithmFlag == 1) && (args->targetCreditsFlag == 1) &&
                    (args->weekdayFlag == 1 && args->periodFlag == 1) );

    if (isAuto == false) return MANUAL; 

    // check if simulated_anneal has all hyperparams
    if (strcmp(args->algorithm, "simulated_anneal") == 0 ||
        strcmp(args->algorithm, "simulated_annealing") == 0 || 
        strcmp(args->algorithm, "siman") == 0 ||  
        strcmp(args->algorithm, "sim_anneal") == 0) {
        args->algorithmCode = SIMAN_ALG;
        if (args->cooldownSetFlag == 1 && args->initTempSetFlag == 1 && args->k_maxSetFlag == 1) return AUTO;
        else {
            fprintf(stderr, "simulated_annealing requires ALL OF --cooldown, --temp, and --k-max to be supplied!\n");
            return ERROR;
        }
    }

    // if there's more algorithms, add more else if here

    else if (strcmp(args->algorithm, "greedy") == 0) {
        args->algorithmCode = GREEDY_ALG;
        return AUTO;
    }

    else {
        fprintf(stderr, "Algorithm '%s' not recognized. Maybe a mistype?\n", args->algorithm);
        return ERROR;
    }
}

int runAuto(const CMDArgs args) {
    // 0 here is good, 1 is bad because it returns all to main
    char filename[MAX_LENGTH];
    char random_file_name[] = "random_course_list.txt";
    Hyperparams hyperparams = fetchHyperparams(args.algorithmCode, args);
    
    // If it's a runAuto either randomFlag OR nameFlag is on
    // We can of course be defensive here and code in a condition
    // to check if something went wrong anyways. Maybe later
    if (args.randomFlag == 1) {
        random_course_list(RANDOM_COURSE_NUMBER, random_file_name);
        strcpy(filename, random_file_name);
    }

    else
        strcpy(filename, args.fileName);

    MasterCheck* mastercheck = createMasterCheck();
    if (mastercheck == NULL) return 1;
    fillMasterCheck(mastercheck, args.targetCredits, args.weekdayCheck, args.periodCheck);

    CourseList* courseList = createCourseList();
    if (courseList == NULL) {
        freeMasterCheck(mastercheck);
        return 1;
    }
    readfile(courseList, filename);

    if (args.algorithmCode == GREEDY_ALG) {
        maximizeCreditsDumb__wrapper(courseList, mastercheck, args.randomSeed, hyperparams, args);
    }

    else if (args.algorithmCode == SIMAN_ALG) {
        simulatedAnnealing__wrapper(courseList, mastercheck, args.initTemp, args.cooldown, 
                                    args.k_max, args.verbose, args.randomSeed, hyperparams, args);
    }

    else {
        fprintf(stderr, "Not sure how you got here in runAuto. Report error to https://github.com/Gumperto/ehs please\n");
        freeMasterCheck(mastercheck);
        freeCourseList(courseList);
        return 1;
    }
    
    freeCourseList(courseList);
    freeMasterCheck(mastercheck);

    return 0;
}

int runManual(char* executablePath, const CMDArgs args) {
    // 0 here is good, 1 is bad because it returns all to main
    char filename[MAX_LENGTH];
    char random_file_name[] = "random_course_list.txt";

    int weekdayCheck[NUM_WEEKDAYS];
    for (int i = MONDAY; i < NUM_WEEKDAYS; i++) weekdayCheck[i] = 1;
    int periodCheck[NUM_PERIODS];
    for (int i = PERIOD_1; i < NUM_PERIODS; i++) periodCheck[i] = 1;

    int targetCredits;
    int algoCode;
    Hyperparams hyper;

    // Resolve name if not
    if (args.randomFlag == 1) {
        random_course_list(RANDOM_COURSE_NUMBER, random_file_name);
        strcpy(filename, random_file_name);
    }

    // earlier in configureRun we set randomFlag to 0 if both are equal to 1
    // could be problematic? fix later
    else if (args.nameFlag == 1) {
        strcpy(filename, args.fileName);              
    }

    else {
        int inputCheck = fetchFilename(executablePath, filename);
        if (inputCheck == 0) return 1;
    }

    CourseList* courseList = createCourseList();
    if (courseList == NULL) return 1;
    readfile(courseList, filename);

    // Resolve the bad slots if not
    resolveAvailabilityChecks(weekdayCheck, periodCheck, args);

    printf("Days remaining: ");
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        if (weekdayCheck[weekday] == 1) printf("%s ", stringNumberToWeek(weekday));

    printf("\n");

    printf("Periods remaining: ");
    for (int period = PERIOD_1; period < NUM_PERIODS; period++)
        if (periodCheck[period] == 1) printf("%d ", period + 1);

    printf("\n\n");

    // Resolve target credit
    if (args.targetCreditsFlag == 1) targetCredits = args.targetCredits;
    else targetCredits = fetchTargetCredits(courseList);

    // Resolve algorithm and its associated hyperparam
    if (args.pickedAlgorithmFlag == 1) algoCode = args.algorithmCode;
    else algoCode = fetchAlgorithm();
    hyper = fetchHyperparams(algoCode, args);

    MasterCheck* mastercheck = createMasterCheck();
    if (mastercheck == NULL) {
        freeCourseList(courseList);
        return 1;
    }
    fillMasterCheck(mastercheck, targetCredits, weekdayCheck, periodCheck);

    if (algoCode == GREEDY_ALG) {
        maximizeCreditsDumb__wrapper(courseList, mastercheck, args.randomSeed, hyper, args);
    }

    else if (algoCode == SIMAN_ALG) {
        simulatedAnnealing__wrapper(courseList, mastercheck, hyper.init_temp_siman, hyper.cooldown_siman,
                                    hyper.k_max, args.verbose, args.randomSeed, hyper, args);
    }

    else {
        fprintf(stderr, "Not sure how you got here in runManual. Report error to https://github.com/Gumperto/ehs please\n");
        freeMasterCheck(mastercheck);
        freeCourseList(courseList);
        return 1;
    }

    freeMasterCheck(mastercheck);
    freeCourseList(courseList);

    return 0;
}
