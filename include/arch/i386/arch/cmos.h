#ifndef KERNEL_ARCH_X86_CMOS_H
#define KERNEL_ARCH_X86_CMOS_H

#include "arch/pio.h"

static inline unsigned char cmos_read(unsigned char address)
{
	outb_wait(0x70, address);
	return inb(0x71);
}

static inline void cmos_write(unsigned char address, unsigned char value)
{
	outb_wait(0x70, address);
	outb_wait(0x71, value);
}

#endif /* ndef KERNEL_ARCH_X86_CMOS_H */
