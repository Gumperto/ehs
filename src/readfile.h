#ifndef READFILE
#define READFILE

#include "structs.h"
void readfile(Schedule* schedule, char* coursesTXT);
Course* createCourse(char* title, char* duration, char* meetings, int credit, int required, char* category);

#endif
