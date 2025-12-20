#include <stdio.h>


unsigned int cMas[10];
unsigned int i;

int Fn(unsigned int uiParam, long *plPtr);

int main(void) {
    long lVar = 100L;  
    int result;

    //* (вариант 15)
    for (i = 0; i < 9; i++) {
        if ((i > 3) && (i != 6))
            cMas[i] = (8 * i) | 0x3;
        else
            cMas[i] = (int)25 * i / 3;
    }

    result = Fn(42, &lVar);

    for (i = 0; i < 9; i++)
        printf("cMas[%u] = %u\n", i, cMas[i]);

    printf("Fn returned: %d, lVar = %ld\n", result, lVar);

    return 0;
}

int Fn(unsigned int uiParam, long *plPtr) {
    long lLocA;        
    int iResult;

    lLocA = (50L - (long)uiParam) * (*plPtr);

    *plPtr = lLocA / 3;

    iResult = (int)(lLocA % 100);

    return iResult;
}