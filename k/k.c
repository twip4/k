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
#include "elf.h"
#include "filesystem.h"
#include "gdt.h"
#include "idt.h"
#include "io/serial.h"
#include "memory.h"
#include "multiboot.h"
#include "sbrk.h"
#include "stdio.h"
#include "syscall.h"
#include "timer.h"

void main_main(unsigned long magic, multiboot_info_t* info)
{
    (void)magic;
    init_serial();
    init_gdt();
    timer_init();
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

    Elf32_Ehdr* elf_header = buff;

    Elf32_Phdr* phdr = (Elf32_Phdr*)((u8*)buff + elf_header->e_phoff);

    for (int i = 0; i < elf_header->e_phnum; i++)
    {
        if (phdr[i].p_type == PT_LOAD)
        {
            printf("segment %d: vaddr=0x%x flags=%s filesz=%d memsz=%d\n", i,
                   phdr[i].p_vaddr, (phdr[i].p_flags & PF_X) ? "CODE" : "DATA",
                   phdr[i].p_filesz, phdr[i].p_memsz);
        }
    }

    for (int i = 0; i < elf_header->e_phnum; i++)
    {
        if (phdr[i].p_type == PT_LOAD)
        {
            memcpy((void*)phdr[i].p_vaddr, buff + phdr[i].p_offset,
                   phdr[i].p_filesz);
            memset((void*)(phdr[i].p_vaddr + phdr[i].p_filesz), 0,
                   phdr[i].p_memsz - phdr[i].p_filesz);
        }
    }

    printf("Game load successful\n");

    void (*entry)(void) = (void (*)(void))elf_header->e_entry;
    entry();
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
