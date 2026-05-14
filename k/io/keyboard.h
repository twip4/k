#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <k/types.h>

#include "k/kstd.h"

#define FIFO_SIZE 256

typedef struct
{
    struct key_event buf[FIFO_SIZE];
    u32 head;
    u32 tail;
    u32 count;
} key_fifo_t;

void handleKeyboard(void);

int readkey(struct key_event* event);

int getkey(void);

#endif /* !KEYBOARD_H */
