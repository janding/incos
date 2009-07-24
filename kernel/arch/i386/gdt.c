#include "arch/gdt.h"
#include "arch/tss.h"

gdt_descriptor_t gdt[] = {
	{ }, // null descriptor
	{ 0xffff, 0x0000, 0x00, GDT_DPL0 | GDT_CODE_SEL, 0xcf, 0x00 }, // kernel code
	{ 0xffff, 0x0000, 0x00, GDT_DPL0 | GDT_DATA_SEL, 0xcf, 0x00 }, // kernel data
	{ 0xffff, 0x0000, 0x00, GDT_DPL3 | GDT_CODE_SEL, 0xcf, 0x00 }, // user code
	{ 0xffff, 0x0000, 0x00, GDT_DPL3 | GDT_DATA_SEL, 0xcf, 0x00 }, // user data
	{
		// tss
		.limit = sizeof(struct tss) - 1, 
		.access = GDT_PRESENT | GDT_TSS,
		.limit16 = ((sizeof(struct tss) >> 16) & 0x0f) | 0x40
	},
};

gdtr_t gdtr = { sizeof(gdt) - 1, (uint32_t)gdt };

void gdt_init() 
{
	__asm__("lgdt %[gdtr]\n\t" 
			"mov %[data_sel], %%ss\n\t"
			"mov %[data_sel], %%ds\n\t"
			"mov %[data_sel], %%es\n\t"
			"mov %[data_sel], %%fs\n\t"
			"mov %[data_sel], %%gs\n\t"
			"jmp %[code_sel], $.+7\n\t"
			:: [gdtr] "m"(gdtr), [code_sel] "i"(0x08), [data_sel] "r"(0x10) : "memory");
}
