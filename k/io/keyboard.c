#include "keyboard.h"

#include <stdio.h>

#include "../io.h"
#include "k/kstd.h"

static const char layoutQWERTY[128] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   '7', '8', '9', '-', '4', '5', '6',
    '+', '1',  '2',  '3', '0',  '.', 0,   0,   0,   0,   0,
};

static char scancode_to_ascii(u8 scanCode)
{
    if (scanCode >= 128)
        return scanCode;

    return layoutQWERTY[scanCode];
}

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

int getkey(void)
{
    struct key_event event;
    readkey(&event);
    return event.key;
}

void handleKeyboard(void)
{
    u8 scancode = inb(0x60);
    u8 press = !(scancode & 0x80);
    struct key_event event = { press ? KEY_PRESSED : KEY_RELEASED,
                               press ? scancode : 0 };
    fifo_push(event);

    // debug
    // if (event.state == KEY_PRESSED)
    // {
    //    printf("%c", scancode_to_ascii(event.key));
    // }
}
