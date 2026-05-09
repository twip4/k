#ifndef VGA_H
#define VGA_H

#include "k/kstd.h"

extern int videomode;

int setvideo(int mode);
void swap_frontbuffer(const void* buffer);

#endif /* !VGA_H */
