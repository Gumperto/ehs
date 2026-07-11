#ifdef __unix__
    #include <unistd.h>
    #include <getopt.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "schedule.h"
#include "readfile.h"
#include "debugCMD.h"
#include "conversion.h"
#include "greedy.h"
#include "random_course_list.h"

#define MAX_LENGTH 256

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
    fgets(filename, MAX_LENGTH, stdin);
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

int starter(int argc, char** argv){
    int opt;

    char filename[MAX_LENGTH];
    char random_file_name[] = "random_courses.txt";

    int targetCredits;
    int weekdayCheck[NUM_WEEKDAYS] = {1, 1, 1, 1, 1, 1};
    int periodCheck[NUM_PERIODS] = {1, 1, 1, 1, 1, 1};

    int inputCheck = 1;

    #ifdef __unix__
        int randomFlag = 0;
        int nameFlag = 0;
        int randomSeed = 1;
        int helpFlag = 0;
        int errFlag = 0;

        while (1) {
            int option_index = 0;
            static struct option long_options[] = 
            {
                {"random-gen",     optional_argument, NULL,  'r'},
                {"data",           required_argument, NULL,  'd'},
                {"help",           no_argument,       NULL,  'h'},
                {NULL,             0,                 NULL,    0}
            };

            int c = getopt_long(argc, argv, "-:r::d:h", long_options, &option_index);
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
                        errFlag = 1;
                        break;
                    }
                    strcpy(filename, optarg);
                    nameFlag = 1;
                    break;

                case 'h':
                    printf("Usage:\t");
                    printf("$ %s [--help] [--data=path] [--random_generated=seed]\n",
                            argv[0]);
                    printf("GUIDE: ");
                    printf("\t%-25s: %-25s\n", "--help OR --h", "print this menu");
                    printf("\t%-25s: %-25s\n", "--data OR --d [path]", "required; tells ehs what file to read");
                    printf("\t%-25s: %-25s\n", "--random-gen OR --r [seed]", "optional; randomly generates a file and reads it");
                    helpFlag = 1;
                    break;
                
                case 'r':
                    randomFlag = 1;
                    break;

                case 'd':
                    if (optarg == NULL) {
                        errFlag = 1;
                        break;
                    }
                    strcpy(filename, optarg);
                    nameFlag = 1;
                    break;

                case '?':
                    printf("Unknown option '%c'\n", optopt);
                    printf("Try:\n");
                    printf("%s --help\n", argv[0]);
                    printf("to see all options\n");
                    errFlag = 1;
                    break;

                case ':':
                    printf("Missing option for '%c'\n", optopt);
                    printf("Try:\n");
                    printf("%s --help\n", argv[0]);
                    printf("to see all options\n");
                    errFlag = 1;
                    break;

                default:
                    printf("?? getopt returned character code 0%o ??\n", c);
                    errFlag = 1;
                    break;
             }
        }

        if (helpFlag == 1 || errFlag == 1) return 1;

        if (randomFlag == 1) {
            random_course_list(random_file_name);
            strcpy(filename, random_file_name);
        }
        
        if (nameFlag != 1 && randomFlag != 1) {
            inputCheck = fetchFilename(argv, filename);
            if (inputCheck == 0) return 1;
        }

        #else
            if (argc == 1) {
                // fetchFilename modifies the argument directly
                inputCheck = fetchFilename(argv, filename);
                if (inputCheck == 0) return 1;
            }
            else if (argc == 2) {
                strcpy(filename, argv[1]);
            }
            
            else {
                strcpy(filename, argv[1]);
                if (strncmp(argv[2], "random", 4) == 0) {
                    random_course_list(random_file_name);
                    strcpy(filename, random_file_name);
                }
            }
    #endif

    CourseList* courseList = createCourseList();
    if (courseList == NULL) return 1;

    printf("Reading from file: %s\n", filename);


    readfile(courseList, filename);

    resolveAvailabilityChecks(weekdayCheck, periodCheck);
    printf("How many credits are you aiming for this semester: ");
    scanf("%d", &targetCredits);

    Schedule* schedule = createSchedule(targetCredits, weekdayCheck, periodCheck);
    if (schedule == NULL) return 1;

    maximizeCreditsDumb(schedule, courseList);
    
    printCourseListInSchedule(schedule);
    printCourseSlotsMatrix(schedule);

    freeCourseList(courseList);
    freeSchedule(schedule);
    
    return 0;
}
