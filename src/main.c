#include <stdio.h>
#include "add.h"

int main() {
    int a = 4, b = 7;
    printf("Hello World!\n");
    printf("Add two numbers with an included function: %d + %d = %d\n", a, b, add(a, b));
    return 0;
}
