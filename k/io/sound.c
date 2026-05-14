#include "sound.h"

#include <stdio.h>

#include "../io.h"
#include "../timer.h"
#include "k/kstd.h"

static struct melody* current_melody = 0;
static int current_note = 0;
static u32 note_end = 0;
static int sound_repeat = 0;

void play_note(u32 freq)
{
    outb(0x43, 0xB6);
    u32 count = 1193182 / freq;
    outb(0x42, count & 0xFF);
    outb(0x42, count >> 8);
    outb(0x61, inb(0x61) | 0x03);
}
void stop_sound()
{
    outb(0x61, inb(0x61) & ~0x03);
}

int playsound(struct melody* melody, int repeat)
{
    if (!melody)
        return -1;
    current_melody = melody;
    current_note = 0;
    sound_repeat = repeat;
    play_note(melody[0].freq);
    note_end = time + (melody[0].duration / 10);
    return 0;
}

void sound_tick(void)
{
    if (!current_melody)
        return;

    if (time < note_end)
        return;

    current_note++;
    if (current_melody[current_note].freq == 0)
    {
        if (sound_repeat == -1)
            current_note = 0;
        else
        {
            stop_sound();
            current_melody = 0;
            return;
        }
    }
    play_note(current_melody[current_note].freq);
    note_end = time + (current_melody[current_note].duration / 10);
}
