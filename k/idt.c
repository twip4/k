#include "idt.h"

#include <stdio.h>

#include "io.h"
#include "io/keyboard.h"
#include "io/serial.h"
#include "syscall.h"
#include "timer.h"

void pic_remap(void)
{
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}

idt_entry_t idt[IDT_MAX_DESCRIPTORS];
idtr_t idtr;

void generic_c_handler(interrupt_frame_t* frame)
{
    if (frame->vector == 0)
    {
        printf("Divide by 0\n");
    }

    if (frame->vector >= 32 && frame->vector < 48)
    {
        if (frame->vector == 32)
        {
            time++; // timer
        }

        if (frame->vector == 33)
        {
            handleKeyboard(); // clavier
        }

        if (frame->vector >= 40)
            outb(0xA0, 0x20); // PIC2
        outb(0x20, 0x20); // PIC1
    }

    // syscall
    if (frame->vector == 128)
    {
        handleSyscall(frame);
    }
}

void idt_set_descriptor(u8 vector, void* isr, u8 flags)
{
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (u32)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (u32)isr >> 16;
    descriptor->reserved = 0;
}

static u8 vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[256];

void idt_init()
{
    pic_remap();

    idtr.base = (u32)&idt[0];
    idtr.limit = (u16)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    // CPU exception
    for (u16 vector = 0; vector < 32; vector++)
    {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = 1;
    }

    // PIC hardware
    for (u16 vector = 32; vector < 48; vector++)
    {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    // syscall ring 3
    idt_set_descriptor(128, isr_stub_table[128], 0xEE);

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");

    printf("IDT init successful\n");
}
