#include <stdio.h>

int main(void) {
    int a = 5 // ERROR: Missing semicolon here.
    int b = 10;

    printf("Value\n", a);
    
    return 0
} // ERROR: Missing semicolon here.