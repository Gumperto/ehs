#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structs.h"
#include "schedule.h"
#include "conversion.h"

#define MAX_SIZE 128

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
    char removedCharacters[] = "{[ ]}\n";
    char replacedCharacters[] = ":";
    char replacementCharacter = ' ';
    removeAllOccurrencesOfChars(string, removedCharacters);
    replaceAllOccurrencesOfChars(string, replacedCharacters, replacementCharacter);
}

void parseWeekdayList(const char *optargStr, int* weekdayCheck) {
    int increment = 0;
    char delimiter[] = ", ";

    for (int i = MONDAY; i < NUM_WEEKDAYS; i++) weekdayCheck[i] = 1;

    char copy[MAX_LENGTH];
    strncpy(copy, optargStr, MAX_LENGTH - 1);
    sanitizeString(copy);
    // because strncpy does not let you have a \0 at the end if you copy exactly MAX_LENGTH chars
    copy[MAX_LENGTH - 1] = '\0';
    
    char* token = strtok(copy, delimiter);
    while (token != NULL && increment < NUM_WEEKDAYS) {
        int weekday = stringWeekToNumber(token);
        if (weekday < MONDAY || weekday >= NUM_WEEKDAYS) {
            fprintf(stderr, "'%s' not recognized as valid day! Skipping...\n", token);
            token = strtok(NULL, delimiter);
            continue;
        }
        printf("Successful! Restricting '%s'\n", stringNumberToWeek(weekday));
        weekdayCheck[weekday] = 0;
        increment++;
        token = strtok(NULL, delimiter);
    }

    printf("\n");
}

void parsePeriodList(const char *optargStr, int* periodCheck) {
    int increment = 0;
    char delimiter[] = ", ";

    for (int i = PERIOD_1; i < NUM_PERIODS; i++) periodCheck[i] = 1;

    char copy[MAX_LENGTH];
    strncpy(copy, optargStr, MAX_LENGTH - 1);
    sanitizeString(copy);
    // because strncpy does not let you have a \0 at the end if you copy exactly MAX_LENGTH chars
    copy[MAX_LENGTH - 1] = '\0';

    char* token = strtok(copy, delimiter);
    while (token != NULL && increment < NUM_PERIODS) {
        int period = atoi(token) - 1; //-1 for index correction
        if (period < PERIOD_1 || period >= NUM_PERIODS) {
            fprintf(stderr, "'%s' not recognized as valid period! Skipping...\n", token);
            token = strtok(NULL, delimiter);
            continue;
        }
        printf("Successful! Restricting period '%s'\n", token);
        periodCheck[period] = 0;
        increment++;
        token = strtok(NULL, delimiter);
    }
    
    printf("\n");
}

// Parse fake occurence dictionaries 
// to a list of integers. For example:
// [Wed: 3; Fri: 1] --> {[WEDNESDAY, 3], [FRIDAY, 1]}
DuoList* parseOccurrences(char* dictionary) {
    // Remove all brackets from string
    DuoList* occurrenceList = createDuoList();
    occurrenceList->list = (Duo*)malloc(MAX_SIZE * sizeof(Duo));
    if (occurrenceList->list == NULL) return NULL;
    occurrenceList->size = 0;
    
    char delimiters[] = ";";
    char *token = strtok(dictionary, delimiters);
 
    // This is actually fine now
    while (token != NULL) {
        if (occurrenceList->size >= MAX_SIZE) {
            destroyDuoList(occurrenceList);
            return NULL;
        }
        char weekday[MAX_LENGTH];
        int period;
        sscanf(token, "%s %d", weekday, &period);
        int numeric_period = period - 1;
        int numeric_weekend = stringWeekToNumber(weekday);
        if (numeric_period >= NUM_PERIODS|| numeric_weekend >= NUM_WEEKDAYS) {
            destroyDuoList(occurrenceList);
            return NULL;
        }

        occurrenceList->list[occurrenceList->size].tuple[PERIOD] = numeric_period; // index correction
        occurrenceList->list[occurrenceList->size].tuple[WEEKDAY] = numeric_weekend;
        occurrenceList->size++;
        token = strtok(NULL, delimiters);
    }

    return occurrenceList;
}
