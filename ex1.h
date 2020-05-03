#ifndef MYMALLOC_H_
#define MYMALLOC_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

typedef struct block
{
    unsigned int totalSize;
    int free;
    struct block *next;
    void *alignedMalloc;
} block;

static block *top, *bot;

void *aligned_malloc(unsigned int size, unsigned int align);

void *aligned_free(void *ptr);

#endif