#ifndef TIMER_H
#define TIMER_H

#include <k/types.h>

extern u32 time;

int gettick(void);

void timer_init();

#endif /* TIMER_H */
