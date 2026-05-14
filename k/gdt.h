#ifndef GDT_H
#define GDT_H

#include <k/types.h>

typedef struct segment
{
    u32 base;
    u32 limit;
    u8 access_byte;
    u8 flags;
} __attribute__((packed)) segment;

struct GDT_entry
{
    u8 entry[8];
} __attribute__((packed));

struct GDTR
{
    u16 limit;
    u32 base;
} __attribute__((packed));

struct tss_entry
{
    u32 prev_tss;
    u32 esp0;
    u32 ss0;
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 cr3, eip, eflags;
    u32 eax, ecx, edx, ebx;
    u32 esp, ebp, esi, edi;
    u32 es, cs, ss, ds, fs, gs;
    u32 ldt;
    u16 trap, iomap_base;
} __attribute__((packed));

void init_gdt(void* stack_end);

#endif /* !GDT_H */
