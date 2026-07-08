#include <stdio.h>
#include <stdlib.h>

//we should work on this thing next, all I did was open and close the file, should this be part of main?
void readfile(char* coursesTXT){
    FILE *fptr;
    fptr = fopen(coursesTXT,"r");
    if(fptr == NULL){
        printf("Error File not found\n");
        exit(1);
    }
    fclose(fptr);
}

