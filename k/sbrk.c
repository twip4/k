#include "sbrk.h"

#include <stdio.h>

#include "memory.h"

static u32 mem_start = 0;
static u32 mem_limit = 0;
static u32 mem_stop = 0;

void heap_init()
{
    mem_start = (u32)memory_reserve(HEAP_SIZE);
    mem_stop = mem_start;
    mem_limit = mem_start + HEAP_SIZE;
    printf("Heap init successful\n");
}

void* sbrk(ssize_t increment)
{
    u32 tmp = mem_stop;

    if (mem_stop + increment <= mem_limit)
    {
        mem_stop += increment;
        return (void*)tmp;
    }

    return 0;
}
