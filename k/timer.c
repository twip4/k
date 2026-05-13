#include "timer.h"

#include "io.h"

u32 time = 0;

int gettick(void)
{
    return time * 10;
}

void timer_init()
{
    u16 divisor = 11931; // ~100 Hz

    outb(0x43, 0x34);

    outb(0x40, divisor & 0xFF);

    outb(0x40, (divisor >> 8) & 0xFF);
}
