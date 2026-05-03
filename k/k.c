/*
 * Copyright (c) LSE
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <k/kstd.h>

#include "gdt.h"
#include "idt.h"
#include "io/serial.h"
#include "memory.h"
#include "multiboot.h"
#include "stdio.h"

void main_main(unsigned long magic, multiboot_info_t* info)
{
    init_serial();
    init_gdt();
    idt_init();

    int res;
    __asm__ volatile("mov $0, %%eax\n"
                     "mov $0, %%ebx\n"
                     "int $0x80\n"
                     "mov %%eax, %0\n"
                     : "=r"(res)
                     :
                     : "eax", "ebx");
    printf("[d] eax=%d\r\n", res);

    (void)magic;
    (void)info;

    char star[5] = "|/-\\";
    char* fb = (void*)0xb8000;

    printf("kernle start\r\n");

    for (unsigned i = 0;;)
    {
        *fb = star[i++ % 4];
    }

    for (;;)
        asm volatile("hlt");
}

void k_main(unsigned long magic, multiboot_info_t* info)
{
    memory_init(info);
    (void)magic;

    void* stack_begin = memory_reserve(0x4000);
    void* stack_end = (char*)stack_begin + 0x4000;

    asm volatile("mov %0, %%esp\n"
                 "mov %0, %%ebp\n"
                 "push %%ecx\n" // info
                 "push %%eax\n" // magic
                 "call main_main\n"
                 "cli\n"
                 "hlt\n"
                 :
                 : "r"(stack_end));
}
