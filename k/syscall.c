#include "syscall.h"

#include <stdio.h>

#include "filesystem.h"
#include "io/keyboard.h"
#include "io/serial.h"
#include "io/vga.h"
#include "k/kstd.h"
#include "k/types.h"
#include "sbrk.h"
#include "timer.h"

u32 sys_test(u32 ebx, u32 ecx, u32 edx);
u32 sys_write(u32 ebx, u32 ecx, u32 edx);
u32 sys_sbrk(u32 ebx, u32 ecx, u32 edx);
u32 sys_getkey(u32 ebx, u32 ecx, u32 edx);
u32 sys_gettick(u32 ebx, u32 ecx, u32 edx);
u32 sys_open(u32 ebx, u32 ecx, u32 edx);
u32 sys_read(u32 ebx, u32 ecx, u32 edx);
u32 sys_lseek(u32 ebx, u32 ecx, u32 edx);
u32 sys_close(u32 ebx, u32 ecx, u32 edx);
u32 sys_setvideo(u32 ebx, u32 ecx, u32 edx);
u32 sys_swap_frontbuffer(u32 ebx, u32 ecx, u32 edx);
u32 sys_setpalette(u32 ebx, u32 ecx, u32 edx);
u32 sys_playsound(u32 ebx, u32 ecx, u32 edx);
u32 sys_getmouse(u32 ebx, u32 ecx, u32 edx);
u32 sys_getkeymode(u32 ebx, u32 ecx, u32 edx);
u32 sys_readkey(u32 ebx, u32 ecx, u32 edx);

handler_function handler_table[NR_SYSCALL];

void handleSyscall(interrupt_frame_t* frame)
{
    // printf("syscall %d\n", frame->eax);
    if (frame->eax >= NR_SYSCALL)
    {
        printf("syscall to high %d\n", frame->eax);
        frame->eax = -1;
        return;
    }

    if (!handler_table[frame->eax])
    {
        printf("syscall not implem %d\n", frame->eax);
        frame->eax = -1;
        return;
    }

    frame->eax = handler_table[frame->eax](frame->ebx, frame->ecx, frame->edx);
}

void syscall_table_init()
{
    for (int i = 0; i < NR_SYSCALL; i++)
    {
        handler_table[i] = 0;
    }

    syscall_register(SYSCALL_WRITE, sys_write);
    syscall_register(SYSCALL_SBRK, sys_sbrk);
    // syscall_register(SYSCALL_GETKEY, sys_getkey);
    syscall_register(SYSCALL_GETTICK, sys_gettick);
    syscall_register(SYSCALL_OPEN, sys_open);
    syscall_register(SYSCALL_READ, sys_read);
    syscall_register(SYSCALL_SEEK, sys_lseek);
    syscall_register(SYSCALL_CLOSE, sys_close);
    syscall_register(SYSCALL_SETVIDEO, sys_setvideo);
    syscall_register(SYSCALL_SWAP_FRONTBUFFER, sys_swap_frontbuffer);
    syscall_register(SYSCALL_SETPALETTE, sys_setpalette);
    syscall_register(SYSCALL_PLAYSOUND, sys_playsound);
    syscall_register(SYSCALL_GETMOUSE, sys_getmouse);
    syscall_register(SYSCALL_READKEY, sys_readkey);
    syscall_register(0, sys_test);

    printf("Syscall init successful\n");
}

void syscall_register(u32 id, handler_function handler)
{
    if (id >= NR_SYSCALL)
        return;

    handler_table[id] = handler;
}

u32 sys_test(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("sys test\n");
    return 42;
}

u32 sys_write(u32 ebx, u32 ecx, u32 edx)
{
    (void)ecx;
    (void)edx;

    // printf("write params %s\n", ebx);

    write_serial(ebx);
    return 0;
}

u32 sys_sbrk(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    // printf("sbrk params %d\n", ebx);

    return (u32)sbrk(ebx);
}

u32 sys_gettick(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    return gettick();
}

u32 sys_open(u32 ebx, u32 ecx, u32 edx)
{
    (void)edx;

    // printf("open params %s, %d, %d\n", ebx, ecx, edx);

    int fd = open((const char*)ebx, ecx);

    // printf("fd %d\n", fd);
    return fd;
}

u32 sys_read(u32 ebx, u32 ecx, u32 edx)
{
    // printf("read params %d, %d, %d\n", ebx, ecx, edx);
    return read(ebx, (void*)ecx, edx);
}

u32 sys_lseek(u32 ebx, u32 ecx, u32 edx)
{
    // printf("seek params %d, %d, %d\n", ebx, ecx, edx);
    int res = seek(ebx, ecx, edx);

    // printf("seek offset %d\n", res);

    return res;
}

u32 sys_close(u32 ebx, u32 ecx, u32 edx)
{
    (void)ecx;
    (void)edx;

    // printf("close fd%d\n", ebx);

    return close(ebx);
}

u32 sys_setvideo(u32 ebx, u32 ecx, u32 edx)
{
    (void)ecx;
    (void)edx;

    // printf("setvideo params %d\n", ebx);

    return setvideo(ebx);
}

u32 sys_swap_frontbuffer(u32 ebx, u32 ecx, u32 edx)
{
    (void)ecx;
    (void)edx;

    swap_frontbuffer((void*)ebx);
    return 0;
}

u32 sys_setpalette(u32 ebx, u32 ecx, u32 edx)
{
    (void)edx;

    set_palette((u32*)ebx, ecx);
    return 0;
}

u32 sys_playsound(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("sys sound\n");
    return 42;
}

u32 sys_getmouse(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("sys mouse\n");
    return 42;
}

u32 sys_readkey(u32 ebx, u32 ecx, u32 edx)
{
    (void)ecx;
    (void)edx;

    struct key_event* key_event = (struct key_event*)ebx;

    int res = readkey(key_event);

    // if (!res)
    //    printf("key code %d, state %d\n", key_event->key, key_event->state);

    return res;
}
