#ifndef KERNEL_ARCH_X86_GDT_H
#define KERNEL_ARCH_X86_GDT_H

#include "types.h"

typedef struct gdtr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdtr_t;


typedef struct gdt_descriptor {
	uint16_t limit;
	uint16_t base;
	uint8_t base16;
	uint8_t access;
	uint8_t limit16;
	uint8_t base24;
} __attribute__((packed)) gdt_descriptor_t;

#define GDT_CODESEG     0x0A
#define GDT_DATASEG     0x02
#define GDT_TSS         0x09
#define GDT_PRESENT     0x80
#define GDT_SEGMENT     0x10

#define GDT_DPL0        0x00
#define GDT_DPL1        0x20
#define GDT_DPL2        0x40
#define GDT_DPL3        0x60

#define GDT_CODE_SEL    (GDT_PRESENT | GDT_SEGMENT | GDT_CODESEG)
#define GDT_DATA_SEL    (GDT_PRESENT | GDT_SEGMENT | GDT_DATASEG)

#endif /* ndef KERNEL_ARCH_X86_GDT_H */
