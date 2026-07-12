#ifndef STARTCMD
#define STARTCMD

#include "structs.h"
int fetchFilename(char** argv, char* filename);
void resolveAvailabilityChecks(CMDArgs args);
int fetchTargetCredits(CourseList* courseList);
CMDArgs cmdlineGetArgs(int argc, char** argv, CMDArgs* args);
int configureRun(int argc, char** argv, CMDArgs* args);

int runAuto(CMDArgs args);
int runManual(char* executablePath, CMDArgs args);

#endif
