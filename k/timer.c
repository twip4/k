#include "timer.h"

u32 time = 0;

int gettick(void)
{
    return time * 10;
}
