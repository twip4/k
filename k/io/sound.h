#ifndef SOUND_H
#define SOUND_H

#include <k/kstd.h>
#include <k/types.h>

int playsound(struct melody* melody, int repeat);
void sound_tick(void);

#endif /* !SOUND_H */
