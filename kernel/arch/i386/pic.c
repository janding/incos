#include "arch/irq.h"
#include "arch/pic.h"
#include "arch/pio.h"

void
pic_init()
{
	outb_wait(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);
	outb_wait(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);

	outb_wait(PIC1_DATA, IRQ_BASE);
	outb_wait(PIC2_DATA, IRQ_BASE + 8);

	outb_wait(PIC1_DATA, 4);
	outb_wait(PIC2_DATA, 2);

	outb_wait(PIC1_DATA, ICW4_8086);
	outb_wait(PIC2_DATA, ICW4_8086);

	outb_wait(PIC1_DATA, 0x00);
	outb_wait(PIC2_DATA, 0x00);
}

void
pic_eoi(unsigned int irq)
{
	outb(PIC1_COMMAND, PIC_EOI);
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
}

int
pic_is_spurious_interrupt(unsigned int irq)
{
	if (irq == 7) {
		outb_wait(PIC1_COMMAND, OCW3_READ_IR);
		return ((inb(PIC1_COMMAND) & 0x80) != 0);
	} else if (irq == 15) {
		outb_wait(PIC2_COMMAND, OCW3_READ_IR);
		return ((inb(PIC2_COMMAND) & 0x80) != 0);
	}

	return 0;
}

void
pic_enable_irq(unsigned int irq)
{
	if (irq < 8)
		outb(PIC1_DATA, inb(PIC1_DATA) | (1 << irq));
	else
		outb(PIC2_DATA, inb(PIC2_DATA) | (1 << (irq - 8)));
}

void
pic_disable_irq(unsigned int irq)
{
	if (irq < 8)
		outb(PIC1_DATA, inb(PIC1_DATA) & ~(1 << irq));
	else
		outb(PIC2_DATA, inb(PIC2_DATA) & ~(1 << (irq - 8)));
}
