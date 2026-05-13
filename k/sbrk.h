#ifndef SBRK_H
#define SBRK_H

#include <k/kstd.h>

#define HEAP_SIZE 0x10000

void* sbrk(ssize_t increment);
void heap_init();

#endif /* !SBRK_H */
