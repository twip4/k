#include "syscall.h"

#include <stdio.h>

#include "io/keyboard.h"
#include "k/types.h"
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
u32 sys_playsound(u32 ebx, u32 ecx, u32 edx);
u32 sys_getmouse(u32 ebx, u32 ecx, u32 edx);
u32 sys_getkeymode(u32 ebx, u32 ecx, u32 edx);
u32 sys_readkey(u32 ebx, u32 ecx, u32 edx);

handler_function handler_table[SYSCALL_SIZE];

void handleSyscall(interrupt_frame_t* frame)
{
    if (frame->eax >= SYSCALL_SIZE)
    {
        frame->eax = -1;
        return;
    }

    if (!handler_table[frame->eax])
    {
        frame->eax = -1;
        return;
    }

    frame->eax = handler_table[frame->eax](frame->ebx, frame->ecx, frame->edx);
}

void syscall_table_init()
{
    for (int i = 0; i < SYSCALL_SIZE; i++)
    {
        handler_table[i] = 0;
    }

    syscall_register(0, sys_write);
    syscall_register(0, sys_sbrk);
    syscall_register(0, sys_getkey);
    syscall_register(0, sys_gettick);
    syscall_register(0, sys_open);
    syscall_register(0, sys_read);
    syscall_register(0, sys_lseek);
    syscall_register(0, sys_close);
    syscall_register(0, sys_setvideo);
    syscall_register(0, sys_swap_frontbuffer);
    syscall_register(0, sys_playsound);
    syscall_register(0, sys_getmouse);
    syscall_register(0, sys_getkeymode);
    syscall_register(0, sys_readkey);
    syscall_register(42, sys_test);

    printf("syscall init\n");
}

void syscall_register(u32 id, handler_function handler)
{
    if (id >= SYSCALL_SIZE)
        return;

    handler_table[id] = handler;
}

u32 sys_test(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_write(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_sbrk(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_getkey(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    return getkey();
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
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_read(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_lseek(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_close(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_setvideo(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_swap_frontbuffer(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_playsound(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_getmouse(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_getkeymode(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}

u32 sys_readkey(u32 ebx, u32 ecx, u32 edx)
{
    (void)ebx;
    (void)ecx;
    (void)edx;

    printf("ptite bite\n");
    return 42;
}
