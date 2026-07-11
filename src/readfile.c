#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"
#include "structs.h"
#include "conversion.h"

#define MAX_LENGTH 512
#define MAX_SIZE 16

void replaceAllOccurrencesOfChars(char* string, char* characterSet, char replacementChar) {
    int len = strlen(string);
    int len_characterSet = strlen(characterSet);
	   	
  	for(int i = 0; i < len; i++){
	    for (int j = 0; j < len_characterSet; j++) {
    		if(string[i] == characterSet[j]) {
    			string[i] = replacementChar;
    		} 
	    }
	}
}

void removeAllOccurrencesOfChars(char* string, char* characterSet) {
    int len = strlen(string);
    int len_characterSet = strlen(characterSet);
	
  	for(int i = 0; i < len; i++) {
	    for (int j = 0; j < len_characterSet; j++) {
    		if(string[i] == characterSet[j]) {
    			for(int k = i; k < len; k++) {
    				string[k] = string[k + 1];
    			}
    			len--;
    			i--;	
    		} 
	    }
	}
}

void sanitizeString(char* string) {
    char removedCharacters[] = "[ ]\n";
    char replacedCharacters[] = ":";
    char replacementCharacter = ' ';
    removeAllOccurrencesOfChars(string, removedCharacters);
    replaceAllOccurrencesOfChars(string, replacedCharacters, replacementCharacter);
}

// Parse fake occurence dictionaries 
// to a list of integers. For example:
// [Wed: 3; Fri: 1] --> {[WEDNESDAY, 3], [FRIDAY, 1]}
DuoList* parseOccurrences(char* dictionary) {
    // Remove all brackets from string
    DuoList* occurrenceList = createDuoList();
    occurrenceList->list = (Duo*)malloc(MAX_SIZE * sizeof(Duo));
    occurrenceList->size = 0;
    
    int increment = 0;
    char delimiters[] = ";";
    char *token = strtok(dictionary, delimiters);
 
    // This is actually fine now
    while (token != NULL) {
        char weekday[MAX_LENGTH];
        int period;
        sscanf(token, "%s %d", weekday, &period);
        int numeric_period = period - 1;
        int numeric_weekend = stringWeekToNumber(weekday);
        if (numeric_period >= NUM_PERIODS|| numeric_weekend >= NUM_WEEKDAYS) return NULL;

        occurrenceList->list[increment].tuple[PERIOD] = numeric_period; // index correction
        occurrenceList->list[increment].tuple[WEEKDAY] = numeric_weekend;
        token = strtok(NULL, delimiters);
        increment++;
        occurrenceList->size++;
    }

    return occurrenceList;
}

Course* setCourse(char* title, char* duration, char* meetings, int credit, int required, char* category) {
    Course* course = createCourse();
    if (course == NULL || title == NULL || duration == NULL || meetings == NULL || category == NULL) {
        destroyCourse(course);
        return NULL;
    }

    // We need a copy here in case we pass the string argument directly into the function.
    // That's going to cause the original char* meetings to be a read-only string literal,
    // resulting in extremely infuriating segfault. Don't make the mistake I did.
    // - Gump
    char meetingsCopy[MAX_LENGTH];
    strcpy(meetingsCopy, meetings);

    Duo durationDuo;
    DuoList* meetingsDuo = parseOccurrences(meetingsCopy);
    if (meetingsDuo == NULL) {
        fprintf(stderr, "Failed to parse meetings for course '%s'\n", course->title);
        destroyCourse(course);
        return NULL;
    }

    if (strcmp(duration, "Q1") == 0) {
        durationDuo.tuple[QUARTER_ONE] = 1;
        durationDuo.tuple[QUARTER_TWO] = 0;
    }
    else if (strcmp(duration, "Q2") == 0) {
        durationDuo.tuple[QUARTER_ONE] = 0;
        durationDuo.tuple[QUARTER_TWO] = 1;
    }
    else if (strcmp(duration, "Semester") == 0) {
        durationDuo.tuple[QUARTER_ONE] = 1;
        durationDuo.tuple[QUARTER_TWO] = 1;
    }
    else {
        destroyCourse(course);
        return NULL;
    }

    course->meetings = meetingsDuo;

    course->title = strdup(title);
    course->category = strdup(category);
    if (course->title == NULL || course->category == NULL) {
        destroyCourse(course);
        return NULL;
    }
    course->durationDuo.tuple[QUARTER_ONE] = durationDuo.tuple[QUARTER_ONE];
    course->durationDuo.tuple[QUARTER_TWO] = durationDuo.tuple[QUARTER_TWO];
    course->credits = credit;
    course->isRequired = required;
    course->takenCredit = 0;
    return course;
}

// Read the file line by line
void readfile(CourseList* courseList, char* coursesTXT){
    FILE *fptr;
    fptr = fopen(coursesTXT,"r");
    if(fptr == NULL){
        printf("Error: Could not open '%s'\n", coursesTXT);
        exit(1);
    }

    int increment = 0;
    char line[MAX_LENGTH];

    // Read the header row
    if (fgets(line, MAX_LENGTH, fptr) == NULL) {
        printf("Error: File is empty.\n");
        fclose(fptr);
        exit(1);
    }

    // Read data from data file
    while (fgets(line, MAX_LENGTH, fptr)) {
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
