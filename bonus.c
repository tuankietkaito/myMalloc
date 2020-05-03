#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct block
{
    unsigned int size;
    int free;
    struct block *next;
} block;

#define SBRK_ERROR (void *)-1

void *top = NULL;

block *find_free_block(block **last, size_t size)
{
    struct block *find = top;
    while (find)
    {
        if ((find->free == 1) && (size <= find->size))
        {
            *last = find;
            return find;
        }
        find = find->next;
    }
    return NULL;
}

// block = <block info> <return address>
block *create_new_block(block *last, size_t size)
{
    block *block;
    block = sbrk(0);
    void *request = sbrk(size + sizeof(struct block));

    if (request == SBRK_ERROR)
        return NULL;

    if (last != NULL)
        last->next = block;

    block->size = size;
    block->next = NULL;
    block->free = 0;

    return block;
}

void *malloc(unsigned int size)
{
    block *block;
    if (size == 0)
        return NULL;

    if (top == NULL)
    {
        {
            block = create_new_block(NULL, size);
            if (block == NULL)
            {
                printf("Top NULL, new Block at: %u\n", block);
                return block;
            }
        }
        top = block;
    }
    else
    {
        struct block *last = top;
        block = find_free_block(&last, size);
        if (!block)
        {
            block = create_new_block(last, size);
            if (!block)
                return NULL;
        }
        else
            block->free = 0;
    }

    return (block + 1);
}

void *free(void *ptr)
{
    if (!ptr)
        return NULL;

    block *block_ptr = (block *)ptr - 1;
    block_ptr->free = 1;
    sbrk(0 - sizeof(struct block) - block_ptr->size);
    return sbrk(0);
}

int main()
{
    int *pA = (int *)malloc(12);
    int *pB = (int *)malloc(20);
    int *pC = (int *)malloc(10);

    free(pB);
    free(pA);
    free(pC);
}