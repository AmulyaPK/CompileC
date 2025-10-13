#include <stdio.h>
#include <stdlib.h>

#define SIZE 8

void print_array(const int arr[], int n) {
    printf("Array elements:\n");
    for (int i = 0; i < n; ++i) {
        printf("[%d]: %d\n", i, arr[i]);
    }
}

double average(const int arr[], int n) {
    if (n <= 0) return 0.0;
    int sum = 0;
    for (int i = 0; i < n; ++i) sum += arr[i];
    return (double) sum / n;
}

void process_array(int arr[], int n) {
    print_array(arr, n);
    double avg = average(arr, n);
    printf("Average: %.2f\n", avg);

    /* Intentionally forget the closing brace for this function to create block error */
    for (int i = 0; i < n; ++i) {
        if (arr[i] < avg) {
            printf("arr[%d] is below average: %d\n", i, arr[i]);
        }
    }
    // Missing closing brace for process_array here


int main(void) {
    int data[SIZE] = {3, 7, 2, 9, 14, 6, 8, 1};
    printf("Program 2: Array stats\n");

    process_array(data, SIZE);

    /* Another deliberate error: malformed printf (missing closing parenthesis)
       to test parser* recovery when encountering tokens within statements. */
    printf("Done processing array\n";  

    return 0;
}