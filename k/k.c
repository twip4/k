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
#include <string.h>

#include "atapi.h"
#include "filesystem.h"
#include "gdt.h"
#include "idt.h"
#include "io/serial.h"
#include "memory.h"
#include "multiboot.h"
#include "sbrk.h"
#include "stdio.h"
#include "syscall.h"

void main_main(unsigned long magic, multiboot_info_t* info)
{
    (void)magic;
    init_serial();
    init_gdt();
    idt_init();
    syscall_table_init();
    atapi_init();
    heap_init();

    printf("kernle start\r\n");

    int fd = open((char*)info->cmdline, 0);

    int size = seek(fd, 0, SEEK_END); // get size and reset
    seek(fd, 0, SEEK_SET);

    void* buff = memory_reserve(size);

    read(fd, buff, size);

    printf("Game load successful\n");

    char star[5] = "|/-\\";
    char* fb = (void*)0xb8000;

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
                 :
                 : "r"(stack_end));

    main_main(magic, info);
}
