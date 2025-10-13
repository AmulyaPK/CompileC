#include <stdio.h>

int main(void) {
    int x = 10;

    if (x == 10) {
        // ERROR: 'break' is not inside a loop or switch.
        break; 
    }

   
    return 0;
}

 // ERROR: 'break' is not inside a loop or switch.
break;