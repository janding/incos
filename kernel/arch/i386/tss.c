#include "arch/gdt.h"
#include "arch/tss.h"

extern gdt_descriptor_t gdt[];

tss_t tss;

void tss_init()
{
	uint32_t addr = (uint32_t)&tss;
	gdt[5].base = addr;
	gdt[5].base16 = addr >> 16;
	gdt[5].base24 = addr >> 24;
	
	tss.ss0 = 0x10;

	__asm__("ltr %%ax\n\t" :: "a"(0x28));
}

