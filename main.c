#include <stdio.h>
#include "myMalloc.h"

int main()
{
    int *pA = (int *)aligned_malloc(12, 5);
    int *pB = (int *)aligned_malloc(12, 4);
    int *pC = (int *)aligned_malloc(12, 4);

    aligned_free(pB);
    aligned_free(pA);
    aligned_free(pC);
}