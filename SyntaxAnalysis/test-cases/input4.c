#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 64

void greet(const char *name);
int count_vowels(const char *s);

void greet(const char *name) {
    if (name == NULL) {
        printf("Hello, stranger!\n");
        return;
    }
    printf("Hello, %s!\n", name);
}

/* Count vowels in a string */
int count_vowels(const char *s) {
    int count = 0;
    for (int i = 0; s[i] != '\0'; ++i) {
        char ch = tolower((unsigned char)s[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') count++;
    }
    return count;
}

int main(void) {
    char name[MAX_NAME] = "Alice";
    greet(name);

    char buffer[MAX_NAME];
    strncpy(buffer, "Testing Parser!", MAX_NAME);
    buffer[MAX_NAME - 1] = '\0';

    int vowels = count_vowels(buffer);
    printf("String: \"%s\"\n", buffer);
    printf("Vowel count: %d\n", vowels);

    /* Intentionally malformed for-loop: missing increment expression
       The grammar should allow recovery using optional_expr -> epsilon. */
    for (int i = 0; i < 3; )  /* missing increment, but allowed as empty */ {
        printf("loop iteration %d\n", i);
        i++;  /* manual increment inside loop body */
    }

    /* Example switch with a malformed case to trigger error recovery.
       One 'case' is intentionally left missing its constant expression. */
    int code = 2;
    switch (code) {
        case 1:
            printf("Case 1\n");
            break;
        case :   /* <-- Intentional syntax error: missing expression after 'case' */
            printf("Malformed case should be recovered\n");
            break;
        default:
            printf("Default case\n");
    }

    /* Multi-line comment with internal * characters to test lexer robustness */
    /*
       End of program 3 demonstration.
       This comment intentionally spans multiple lines to test lexical handling.
    */

    return 0;
}