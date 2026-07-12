#ifdef __unix__
    #include <unistd.h>
    #include <getopt.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "schedule.h"
#include "readfile.h"
#include "debugCMD.h"
#include "conversion.h"
#include "greedy.h"
#include "randomness.h"
#include "random_course_list.h"
#include "objective.h"
#include "simulated_annealing.h"

int fetchFilename(char** argv, char* filename) {
    int check;

    printf("You are inputting filename manually.\n");
    printf("Pro-tip: you can use cmdline arguments with ehs!\n");
    printf("For example, type \n");
    printf("\t%s {filename}\n", argv[0]);
    printf("to automatically use that argument as the filename!\n");
    #ifdef __unix__
        printf("\nTo see more other commandline stuff, try:\n");
        printf("\t%s --help\n\n", argv[0]);
    
    #else
        printf("You can also randomly generate a file and use that by trying:\n");
        printf("\t%s {filename} random\n\n", argv[0]);
    #endif

    printf("Input filename manually: ");

    // Actually scan the filename
    fgets(filename, MAX_LENGTH - 1, stdin);
    check = sscanf(filename, "%s", filename);
    if (check != 1) {
        printf("Please type at least one input.\n");
        return 0;
    }
    return 1;
}

void resolveAvailabilityChecks(int* weekdayCheck, int* periodCheck) {
    char temp[MAX_LENGTH];
    char delimiter[] = " ";

    // Read into periodCheck
    printf("Input all periods you do not want to attend, separated by spaces: ");
    fgets(temp, MAX_LENGTH, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    
    int increment = 0;
    char* token = strtok(temp, delimiter);
    while(token != NULL && increment < NUM_PERIODS) {
        int numeric_period = atoi(token) - 1; // the -1 is index correction
        
        // If out of range
        if (numeric_period >= NUM_PERIODS || numeric_period < PERIOD_1) {
            printf("Failure!\n");
            token = strtok(NULL, delimiter);
            continue;
        }
        printf("Success!\n");
        periodCheck[numeric_period] = 0;
        increment++; // add only after a valid check
        token = strtok(NULL, delimiter);
    }

    // Read into weekdayCheck
    printf("Input all weekdays you do not want to attend, separated by spaces: ");
    fgets(temp, MAX_LENGTH, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    
    increment = 0;
    token = strtok(temp, delimiter);
    while(token != NULL && increment < NUM_WEEKDAYS) {
        int numeric_weekday = stringWeekToNumber(token);
        
        // If out of range
        if (numeric_weekday < MONDAY || numeric_weekday >= NUM_WEEKDAYS) {
            printf("Failure!\n");
            token = strtok(NULL, delimiter);
            continue;
        }
        printf("Success!\n");
        weekdayCheck[numeric_weekday] = 0;
        increment++;
        token = strtok(NULL, delimiter);
    }

    printf("\n");

    printf("Days remaining: ");
    for (int weekday = MONDAY; weekday < NUM_WEEKDAYS; weekday++)
        if (weekdayCheck[weekday] == 1) printf("%s ", stringNumberToWeek(weekday));

    printf("\n");

    printf("Periods remaining: ");
    for (int period = PERIOD_1; period < NUM_PERIODS; period++)
        if (periodCheck[period] == 1) printf("%d ", period + 1);

    printf("\n\n");
}

int fetchTargetCredits(CourseList* courseList) {
    int targetCredits = -1;
    char buffer[MAX_LENGTH];
    printf("How many credits are you aiming for this semester: ");
    
    // Actually scan the target credits
    fgets(buffer, MAX_LENGTH - 1, stdin);
    int check = sscanf(buffer, "%d", &targetCredits);

    // If no input was given or if the input sucks
    if (check != 1) {
        int courseCap = min(courseList->courseCountTotal, RANDOM_COURSE_NUMBER);
        int totalCreditsInCourseList = 0;
        for (int course = 0; course < courseCap; course++)
            totalCreditsInCourseList += courseList->courseList[course]->credits;

        // 8 semesters
        targetCredits = totalCreditsInCourseList / 8;

        printf("Using very rough estimate: %d\n\n", targetCredits);
    }

    return targetCredits;
}

CMDArgs cmdlineGet(int argc, char** argv) {
    CMDArgs args;
    args.randomFlag = 0;
    args.nameFlag = 0;
    args.helpFlag = 0;
    args.errFlag = 0;
    args.verbose = 0;
    args.randomSeed = 42;


    #ifdef __unix__
        

        while (1) {
            int option_index = 0;
            static struct option long_options[] = 
            {
                {"random-gen",     optional_argument, NULL,  'r'},
                {"data",           required_argument, NULL,  'd'},
                {"help",           no_argument,       NULL,  'h'},
                {"verbose",        no_argument,       NULL,  'v'},
                {NULL,             0,                 NULL,    0}
            };

            int c = getopt_long(argc, argv, "-:r::d:hv", long_options, &option_index);
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
                    printf("GUIDE: ");
                    printf("\t%-28s: %-28s\n", "--help OR --h", "print this menu");
                    printf("\t%-28s: %-28s\n", "--verbose OR --v", "print detailed execution results");
                    printf("\t%-28s: %-28s\n", "--data OR --d [path]", "arg required; tells ehs what file to read");
                    printf("\t%-28s: %-28s\n", "--random-gen OR --r [seed]", "arg optional; randomly generates a file and reads it");
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

                case 'v':
                    args.verbose = 1;
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

int starter(int argc, char** argv){
    // seeds random
    srand(time(NULL));
    int inputCheck = 1;

    char random_file_name[] = "random_courses.txt";
    int targetCredits;

    int weekdayCheck[NUM_WEEKDAYS] = {1, 1, 1, 1, 1, 1};
    int periodCheck[NUM_PERIODS] = {1, 1, 1, 1, 1, 1};

    CMDArgs arguments = cmdlineGet(argc, argv);
    if (arguments.helpFlag == 1 || arguments.errFlag == 1) return 1;
    else {
        if (arguments.randomFlag == 1) {
            random_course_list(RANDOM_COURSE_NUMBER, random_file_name);
            strcpy(arguments.fileName, random_file_name);
        }
        
        else {
            if (arguments.nameFlag != 1) {
                inputCheck = fetchFilename(argv, arguments.fileName);
                if (inputCheck == 0) return 1;
            }        
        }
    }

    CourseList* courseList = createCourseList();
    if (courseList == NULL) return 1;

    printf("Reading from file: %s\n", arguments.fileName);

    readfile(courseList, arguments.fileName);

    resolveAvailabilityChecks(weekdayCheck, periodCheck);
    targetCredits = fetchTargetCredits(courseList);

    MasterCheck* mastercheck = createMasterCheck();
    if (mastercheck == NULL) return 1;
    fillMasterCheck(mastercheck, targetCredits, weekdayCheck, periodCheck);

    Schedule* dumbSchedule = createSchedule();
    if (dumbSchedule == NULL) return 1;

    maximizeCreditsDumb(dumbSchedule, courseList, mastercheck);
    printf("Objective of dumb schedule: %lf\n", objective(dumbSchedule, courseList, mastercheck, arguments.verbose));

    printCourseSlotsMatrix(dumbSchedule);
    freeSchedule(dumbSchedule);

    Schedule* annealSchedule = simulatedAnnealing(
                                courseList, 
                                mastercheck, 
                                10000, 
                                0.995,
                                arguments.verbose);
    if (annealSchedule == NULL) return 1;

    printCourseSlotsMatrix(annealSchedule);
    freeSchedule(annealSchedule);

    freeCourseList(courseList);
    freeMasterCheck(mastercheck);
    
    return 0;
}
