#include "gdt.h"

#include <stdio.h>

#include "memory.h"
#include "string.h"

struct GDT_entry GDT[6];
struct GDTR gdtr;
struct tss_entry tss;

void encodeGdtEntry(u8* target, struct segment source)
{
    // Check the limit to make sure that it can be encoded
    if (source.limit > 0xFFFFF)
    {
        printf("GDT cannot encode limits larger than 0xFFFFF");
        return;
    }

    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;

    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // Encode the access byte
    target[5] = source.access_byte;

    // Encode the flags
    target[6] |= (source.flags << 4);
}

void init_gdt(void* stack_end)
{
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = 0x10;
    tss.esp0 = (u32)stack_end;
    tss.iomap_base = sizeof(tss);

    segment null_desc = { 0, 0, 0, 0 };
    segment kernel_code_desc = { 0, 0xFFFFF, 0x9A, 0xCF };
    segment kernel_data_desc = { 0, 0xFFFFF, 0x92, 0xCF };
    segment user_code_desc = { 0, 0xFFFFF, 0xFA, 0xCF };
    segment user_data_desc = { 0, 0xFFFFF, 0xF2, 0xCF };
    segment tss_desc = { (u32)&tss, sizeof(tss) - 1, 0x89, 0x00 };

    encodeGdtEntry(GDT[0].entry, null_desc);
    encodeGdtEntry(GDT[1].entry, kernel_code_desc);
    encodeGdtEntry(GDT[2].entry, kernel_data_desc);
    encodeGdtEntry(GDT[3].entry, user_code_desc);
    encodeGdtEntry(GDT[4].entry, user_data_desc);
    encodeGdtEntry(GDT[5].entry, tss_desc);

    gdtr.base = (u32)GDT;
    gdtr.limit = sizeof(GDT) - 1;

    // charge la GDT
    asm volatile("lgdt %0\n"
                 : /* no output */
                 : "m"(gdtr)
                 : "memory");

    // reset les registres memoire de la GDT et jump code segment
    asm volatile("mov $0x10, %%ax \n\t"
                 "mov %%ax, %%ds \n\t"
                 "mov %%ax, %%es \n\t"
                 "mov %%ax, %%fs \n\t"
                 "mov %%ax, %%gs \n\t"
                 "mov %%ax, %%ss \n\t"
                 "ljmp $0x08, $.flush \n\t" // far jump
                 ".flush: \n\t"
                 :
                 :
                 : "ax");

    asm volatile("ltr %%ax" : : "a"((u16)0x28));

    printf("GDT init successful\n");
}
