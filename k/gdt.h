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

void init_gdt();

#endif /* !GDT_H */
