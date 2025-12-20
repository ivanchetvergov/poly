#include <stdio.h>

unsigned int cMas[10];
unsigned int i;

// *  вариант 3.15
int main(void) {
    for (i = 0; i < 9; i++) {
        if ((i > 3) && (i != 6))
            cMas[i] = (8 * i) | 0x3;
        else
            cMas[i] = (int)25 * i / 3;
    }

    for (i = 0; i < 9; i++)
        printf("cMas[%u] = %u\n", i, cMas[i]);

    return 0;
}