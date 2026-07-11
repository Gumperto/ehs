#include <string.h>
#include <stdlib.h>
#include "structs.h"

int comp_credits(const void * a, const void * b){
    return(((*(const Course **)b)->credits) - ((*(const Course **)a)->credits));
}

int comp_required(const void * a, const void * b){
    return(((*(const Course **)b)->isRequired) - ((*(const Course **)a)->isRequired));
}

int comp_name(const void * a, const void * b){
    return( strcmp( ((*(const Course **)a)->title), ((*(const Course **)b)->title) ) );
}

int comp_category(const void * a, const void * b){
    return( strcmp( ((*(const Course **)a)->category), ((*(const Course **)b)->category) ) );
}

/* Sorts a CourseList by 4 (subject to change) main criteria:
 * - Credits: H -> L
 * - Required: R -> NR
 * - Name: A -> Z
 * - Category: A -> Z
 *   */
void sortCourseList(CourseList* courseList, char* criterion) {
    size_t first_comparison_chars = 3;
    if (strncmp(criterion, "credit", first_comparison_chars) == 0)
        qsort(courseList->courseList, courseList->courseCountTotal, sizeof(Course*), comp_credits);
    else if (strncmp(criterion, "required", first_comparison_chars) == 0)
        qsort(courseList->courseList, courseList->courseCountTotal, sizeof(Course*), comp_required);
    else if (strncmp(criterion, "name", first_comparison_chars) == 0)
        qsort(courseList->courseList, courseList->courseCountTotal, sizeof(Course*), comp_name);
    else if (strncmp(criterion, "category", first_comparison_chars) == 0)
        qsort(courseList->courseList, courseList->courseCountTotal, sizeof(Course*), comp_category);
    else
        return;
}
