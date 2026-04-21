#include "keyboard.h"

#include <stdio.h>

#include "../io.h"

void fifo_push(struct key_event event)
{
    if (fifo.count == FIFO_SIZE)
        return; // pleine
    fifo.buf[fifo.tail] = event;
    fifo.tail = (fifo.tail + 1) % FIFO_SIZE;
    fifo.count++;
}

int readkey(struct key_event* event)
{
    if (fifo.count == 0)
        return 0; // vide
    *event = fifo.buf[fifo.head];
    fifo.head = (fifo.head + 1) % FIFO_SIZE;
    fifo.count--;
    return 1;
}

void handleKeyboard(void)
{
    u8 scancode = inb(0x60);
    struct key_event event = { scancode & 0x80 ? KEY_PRESSED : KEY_RELEASED,
                               scancode };
    fifo_push(event);

    // debug
    printf("touch press %d, scancode %d\n", event.state, event.key);
}
