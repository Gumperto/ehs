#ifndef STRINGOPS
#define STRINGOPS

#include "structs.h"
void replaceAllOccurrencesOfChars(char* string, char* characterSet, char replacementChar);
void removeAllOccurrencesOfChars(char* string, char* characterSet);
void sanitizeString(char* string);

void parseWeekdayList(const char *optargStr, int* weekdayCheck);
void parsePeriodList(const char *optargStr, int* periodCheck);

DuoList* parseOccurrences(char* dictionary);

#endif
