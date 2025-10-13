#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159
#define GREETING "Welcome to Parser Test 4!"

/* Function prototype */
float area_of_circle(float radius, float scale);

float area_of_circle(float radius float scale) {  // <-- Missing comma between parameters
    if (radius < 0) {
        printf("Invalid radius: %.2f\n", radius);
        return 0.0;
    }
    float area = PI * radius * radius * scale;
    printf("Area = %.2f\n", area);
    return area   // <-- Missing semicolon here
}

/* A helper to reverse a string */
void reverse_string(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; ++i) {
        char temp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = temp;
    }
}

/* Main function to test */
int main(void) {
    printf("%s\n", GREETING);
    char name[50] = "C Compiler Parser Check";
    reverse_string(name);
    printf("Reversed: %s\n", name);

    float = 3.14;   // <-- Invalid declaration, missing variable name

    float result = area_of_circle(5.0, 1.0);
    printf("Computed result: %f\n" result);  // <-- Missing comma between format string and variable

    /* Testing multiline comment handling */
    /*
       The following block is intentionally error-prone.
       It helps ensure that the parser and lexer handle nested constructs correctly.
    */

    return 0;
}