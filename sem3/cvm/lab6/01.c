#include <stdio.h>

unsigned int cMas[10];
unsigned int i;

int main(void) {
    unsigned int *p;
    i = 0;
    p = cMas; 
 
    // * do while 
    do {
        if ((i > 3) && (i != 6))
            *p = (8 * i) | 0x3;
        else
            *p = (int)25 * i / 3;

        i++;
        p++; 
    } while (i < 9);

    for (i = 0; i < 9; i++)
        printf("cMas[%u] = %u\n", i, cMas[i]);

    return 0;
}