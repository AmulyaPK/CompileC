#include <stdio.h>

struct Point {
    float x;
    float y;
};

union Coordinate {
    float x;
    float y;
};

int main() {
    struct Point myPoint;
    union Coordinate coord;
    
    myPoint.x = 3.0;
    myPoint.y = 4.0;

    printf("Coords: (%.2f, %.2f)\n", myPoint.x, myPoint.y);
    coord.x = myPoint.x;
    printf("X-value: %.2f\n", coord.x);
    coord.y = myPoint.y;
    printf("Y-value: %.2f\n", coord.y);

    return 0;
}