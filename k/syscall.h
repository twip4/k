#ifndef SYSCALL_H
#define SYSCALL_H

#include "idt.h"

void handleSyscall(interrupt_frame_t* frame);

#endif /* !SYSCALL_H */
