#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"
#include "structs.h"
#include "stringOps.h"

#define MAX_LINE_LENGTH 512

// Read the file line by line
void readfile(CourseList* courseList, char* coursesTXT){
    FILE *fptr;
    fptr = fopen(coursesTXT,"r");
    if(fptr == NULL){
        printf("Error: Could not open '%s'\n", coursesTXT);
        exit(1);
    }

    int increment = 0;
    char line[MAX_LINE_LENGTH];

    // Read the header row
    if (fgets(line, MAX_LINE_LENGTH, fptr) == NULL) {
        printf("Error: File is empty.\n");
        fclose(fptr);
        exit(1);
    }

    // Read data from data file
    while (fgets(line, MAX_LINE_LENGTH, fptr)) {
        // Read and discard the ID column (if exists)
        // char *token = strtok(line, ",");
        // if (!token) continue;
        
        char title[MAX_LENGTH] = "";
        char duration[MAX_LENGTH] = "";
        char meetings[MAX_LENGTH] = "";
        int credit = -1;
        int required = -1;
        char category[MAX_LENGTH] = "";

        // Read name
        char* token = strtok(line, ",");
        if (token == NULL) continue;
        else {
            sanitizeString(token);
            strcpy(title, token);
        }

        // Read duration
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        else {
            sanitizeString(token);
            strcpy(duration, token);
        }

        // Get weekly freq (doesn't do anything for now)
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        
        // Get occurrences
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        else {
            sanitizeString(token);
            strcpy(meetings, token);
        }

        // Get credits
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        else {
            sanitizeString(token);
            credit = atoi(token);
        }

        // Get isRequired
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        else {
            sanitizeString(token);
            required = atoi(token);
        }

        // Get category
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        else {
            sanitizeString(token);
            strcpy(category, token);
        }

        if (strcmp(title, "") == 0 ||  strcmp(duration, "") == 0 || strcmp(meetings, "") == 0 || 
            credit == -1 || required == -1 || strcmp(category, "") == 0) {
            // ignores the row if anything is read incorrectly
            continue;
        }

        else {
            Course* course = setCourse(title, duration, meetings, credit, required, category);
            bool temp = pushCourseList(courseList, course);
            if (temp == false || course == NULL) {
                printf("List not read correctly!\n");
                exit(1);
            }
        }
    }

    fclose(fptr);
}
