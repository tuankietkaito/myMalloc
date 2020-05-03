#include <stdio.h>
#include "myMalloc.h"

int main()
{
    printf("Block A\n");
    int *pA = (int *)aligned_malloc(12, 5);
    printf("Block B\n");
    int *pB = (int *)aligned_malloc(12, 4);
    printf("Block C\n");
    int *pC = (int *)aligned_malloc(12, 4);

    printf("Free Block A\n");
    aligned_free(pB);
    printf("Free Block B\n");
    aligned_free(pA);
    printf("Free Block C\n");
    aligned_free(pC);
}