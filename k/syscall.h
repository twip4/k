#ifndef SYSCALL_H
#define SYSCALL_H

#include "idt.h"
#include "k/kstd.h"
#include "k/types.h"

typedef u32 (*handler_function)(u32 ebx, u32 ecx, u32 edx);

extern handler_function handler_table[NR_SYSCALL];

void handleSyscall(interrupt_frame_t* frame);

void syscall_table_init();

void syscall_register(u32 id, handler_function handler);

#endif /* !SYSCALL_H */
