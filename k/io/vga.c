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
