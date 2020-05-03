#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#define SBRK_ERROR (void *)-1

#include "myMalloc.h"

block *find_free_block(unsigned int size, unsigned int align)
{
    block *find = top;

    while (find != NULL)
    {
        unsigned int ptr = (unsigned int)find;
        unsigned int empty = (ptr + sizeof(unsigned int)) % align;
        empty = (empty > 0) ? align - empty : empty;

        if ((find->free == 1) && (size + empty + sizeof(empty) <= find->totalSize))
            return find;

        find = find->next;
    }

    return NULL;
}

// block = <block info> <empty> <value of empty> <return address>
void *create_new_block(unsigned int size, unsigned int align)
{
    void *returnBlock;
    unsigned int addr, empty;

    returnBlock = sbrk(0);

    if (sbrk(sizeof(block)) == SBRK_ERROR)
        return NULL;

    addr = (unsigned int)sbrk(0);
    empty = (addr + sizeof(empty)) % align;
    empty = (empty > 0) ? align - empty : empty;

    if (sbrk(empty) == SBRK_ERROR)
        return NULL;

    if (sbrk(sizeof(empty)) == SBRK_ERROR)
        return NULL;
    *((unsigned int *)sbrk(0) - 1) = empty;
    // printf("Empty at %d is %d, ", ((unsigned int)sbrk(0) - 1), empty);

    block *blockInfo = (block *)returnBlock;
    blockInfo->alignedMalloc = (void *)sbrk(0);
    if (sbrk(size) == SBRK_ERROR)
        return NULL;

    blockInfo->totalSize = size + empty + sizeof(empty);
    blockInfo->free = 0;
    blockInfo->next = NULL;

    return returnBlock;
}

// Check if Align is power of 2
int checkAlign(unsigned int x)
{
    if (x == 0)
        return 0;

    while (x != 1)
    {
        if (x % 2 != 0)
            return 0;
        x /= 2;
    }
    return 1;
}

void *aligned_malloc(unsigned int size, unsigned int align)
{
    if (size == 0 || align == 0 || checkAlign(align) == 0)
        return NULL;

    block *tmpBlock;

    tmpBlock = find_free_block(size, align);
    if (tmpBlock != NULL)
    {
        unsigned int addr, empty;
        tmpBlock->free = 0;

        // Set empty
        addr = (unsigned int)(tmpBlock + 1);
        empty = (addr + sizeof(empty)) % align;
        empty = (empty > 0) ? align - empty : empty;

        // Set sizeof(empty)
        addr = (unsigned int)(tmpBlock + 1) + empty;
        unsigned int *emptySize = (unsigned int *)addr;
        *emptySize = empty;

        // Set return address
        addr = (unsigned int)(tmpBlock + 1) + empty + sizeof(empty);
        tmpBlock->alignedMalloc = (void *)addr;

        printf("New malloc at %u\n", (unsigned int)tmpBlock->alignedMalloc);
        return tmpBlock->alignedMalloc;
    }

    tmpBlock = create_new_block(size, align);

    if (tmpBlock == NULL)
        return NULL;

    if (!top)
        top = tmpBlock;
    if (bot)
        bot->next = tmpBlock;
    bot = tmpBlock;

    printf("New malloc at %u\n", (unsigned int)tmpBlock->alignedMalloc);
    return tmpBlock->alignedMalloc;
}

void *aligned_free(void *ptr)
{
    if (!ptr)
        return NULL;

    unsigned int emptySize = *((unsigned int *)(ptr)-1);
    unsigned int addr = (unsigned int)ptr - emptySize - sizeof(unsigned int) - sizeof(block);
    block *freeBlock = (block *)addr;

    if (freeBlock == bot)
    {
        if (top == bot)
        {
            top = NULL;
            bot = NULL;
        }
        else
        {
            block *tmp = top;
            while (tmp->next != bot)
                tmp = tmp->next;
            bot = tmp;
            bot->next = NULL;
        }

        // Return the address of the <block info> in block needed to be free
        sbrk(0 - sizeof(block) - freeBlock->totalSize);
        printf("Block free at %u\n", freeBlock->alignedMalloc);

        return sbrk(0);
    }
    else
    {
        freeBlock->free = 1;
        printf("Block Free at : %u\n", freeBlock->alignedMalloc);
        return sbrk(0);
    }
}