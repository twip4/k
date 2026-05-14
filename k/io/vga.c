#include "vga.h"

#include <string.h>

#include "../io.h"
#include "../libvga.h"
#include "k/kstd.h"

int videomode = VIDEO_TEXT;

int setvideo(int mode)
{
    if (mode == VIDEO_TEXT)
    {
        videomode = mode;

        libvga_switch_mode3h();

        return 0;
    }

    if (mode == VIDEO_GRAPHIC)
    {
        videomode = mode;

        libvga_switch_mode13h();

        return 0;
    }

    return -1;
}

void swap_frontbuffer(const void* buffer)
{
    if (videomode == VIDEO_GRAPHIC)
    {
        memcpy((void*)0xA0000, buffer, 64000);
    }
}

void set_palette(u32* palette, size_t size)
{
    outb(0x3C8, 0); // commencer à l'index 0
    for (size_t i = 0; i < size; i++)
    {
        outb(0x3C9, ((palette[i] >> 16) & 0xFF) >> 2); // R
        outb(0x3C9, ((palette[i] >> 8) & 0xFF) >> 2); // G
        outb(0x3C9, ((palette[i] >> 0) & 0xFF) >> 2); // B
    }
}
