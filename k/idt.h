#ifndef IDT_H
#define IDT_H

#include <k/types.h>

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define IDT_MAX_DESCRIPTORS 256

typedef struct
{
    u16 isr_low;
    u16 kernel_cs;
    u8 reserved;
    u8 attributes;
    u16 isr_high;

} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) extern idt_entry_t idt[IDT_MAX_DESCRIPTORS];

typedef struct
{
    u16 limit;
    u32 base;
} __attribute__((packed)) idtr_t;

extern idtr_t idtr;

void idt_init(void);

#endif /* !IDT_H */
