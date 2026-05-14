#include "keyboard.h"

#include <stdio.h>

#include "../io.h"
#include "k/kstd.h"

static int shift_pressed = 0;
static int caps_lock = 0;

static key_fifo_t fifo = { 0 };

static const char layoutQWERTY[128] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   '7', '8', '9', '-', '4', '5', '6',
    '+', '1',  '2',  '3', '0',  '.', 0,   0,   0,   0,   0,
};

static const char layoutQWERTYShift[128] = {
    0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    '\n', 0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0,    '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*',
    0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static char scancode_to_ascii(u8 scancode)
{
    if (scancode >= 128)
        return 0;

    char c;

    if (shift_pressed)
        c = layoutQWERTYShift[scancode];
    else
        c = layoutQWERTY[scancode];

    // CapsLock seulement sur lettres
    if (caps_lock && c >= 'a' && c <= 'z')
        c -= 32;
    else if (caps_lock && c >= 'A' && c <= 'Z')
        c += 32;

    return c;
}

void fifo_push(struct key_event event)
{
    if (fifo.count >= FIFO_SIZE)
    {
        printf("keyboard full\n");
        return; // pleine
    }
    fifo.buf[fifo.tail] = event;
    fifo.tail = (fifo.tail + 1) % FIFO_SIZE;
    fifo.count++;
}

int readkey(struct key_event* event)
{
    if (fifo.count == 0)
        return 1; // vide
    *event = fifo.buf[fifo.head];
    fifo.head = (fifo.head + 1) % FIFO_SIZE;
    fifo.count--;
    return 0;
}

int getkey(void)
{
    struct key_event event;

    while (1)
    {
        if (readkey(&event) == 0)
        {
            if (event.state == KEY_PRESSED && event.key)
                return event.key;
        }
    }
}

void handleKeyboard(void)
{
    u8 scancode = inb(0x60);

    // Ignore extended keys
    if (scancode == 0xE0)
        return;

    u8 released = scancode & 0x80;
    u8 keycode = scancode & 0x7F;

    // Shift
    if (keycode == 0x2A || keycode == 0x36)
    {
        shift_pressed = !released;
        return;
    }

    // CapsLock
    if (keycode == 0x3A && !released)
    {
        caps_lock = !caps_lock;
        return;
    }

    struct key_event event = { released ? KEY_RELEASED : KEY_PRESSED, keycode };

    fifo_push(event);

    // debug
    /*

    char ascii = scancode_to_ascii(keycode);
    if (!released && ascii)
        printf("%c", ascii);
    */
}
