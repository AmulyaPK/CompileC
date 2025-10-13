#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAL 100
#define MIN_VAL 0

/* compute_gcd: returns gcd of a and b */
int compute_gcd(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int add_and_check(int x, int y) {
    int s = x + y;
    if (s > MAX_VAL) {
        printf("Warning: sum %d exceeds MAX_VAL (%d)\n", s, MAX_VAL);
    }
    return s;
}

int main(void) {
    printf("Program 1: Math utilities test\n");

    int a = 12;
    int b = 18;
    int g = compute_gcd(a, b);
    printf("gcd(%d, %d) = %d\n", a, b, g);

    int sum = add_and_check(a, b);
    printf("sum = %d\n", sum);

    int c = 7     // <-- Intentional error: missing semicolon here
    int d = 3;
    int result = c + * d;  /* Intentional malformed expression to trigger parser error */
    printf("result = %d\n", result);

    /* A small loop */
    for (int i = 0; i < 5; ++i) {
        printf("i = %d\n", i);
    }

    return 0;
}