#include "syscall.h"

void handleSyscall(interrupt_frame_t* frame)
{
    switch (frame->eax)
    {
    default:
        frame->eax = -1;
        break;
    }
}
