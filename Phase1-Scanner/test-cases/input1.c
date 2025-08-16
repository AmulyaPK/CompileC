#include <stdio.h>
#define SIZE 5   // Array size

/* Multi-line comment
   describing the program */

int globalVar = 10;   // Global variable

// Function declaration
void printMessage(const char *msg) {
    printf("%s\n", msg);
}

// Function with array parameter
int sumArray(int nums[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += nums[i];
    }
    return sum;
}

int main() {
    int arr[SIZE] = {1, 2, 3, 4, 5};
    printMessage("Hello World");

    int localSum = sumArray(arr, SIZE);
    printf("Sum = %d\n", localSum);

    return 0;
}
