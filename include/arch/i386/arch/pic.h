#ifndef KERNEL_ARCH_PIC_H
#define KERNEL_ARCH_PIC_H

#define PIC1_BASE		0x20			// IO base address for master PIC
#define PIC2_BASE		0xA0			// IO base address for slave PIC
#define PIC1_COMMAND	PIC1_BASE
#define PIC1_DATA		(PIC1_BASE+1)
#define PIC2_COMMAND	PIC2_BASE
#define PIC2_DATA		(PIC2_BASE+1)

#define ICW1_ICW4		0x01			// ICW4 (not) needed
#define ICW1_SINGLE		0x02			// Single (cascade) mode
#define ICW1_INTERVAL4	0x04			// Call address interval 4 (8)
#define ICW1_LEVEL		0x08			// Level triggered (edge) mode
#define ICW1_INIT		0x10			// Initialization

#define ICW4_8086		0x01			// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO		0x02			// Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08			// Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C			// Buffered mode/master
#define ICW4_SFNM		0x10			// Special fully nested (not)

#define OCW2_EOI		0x20			// End of interrupt
#define OCW2_SL			0x40
#define OCW2_ROTATE		0x80	

#define OCW3			0x08
#define OCW3_POLL		(OCW3 | 0x04)
#define OCW3_READ_REG	(OCW3 | 0x02)
#define OCW3_READ_IR	(OCW3_READ_REG | 0x01)
#define OCW3_READ_IS	(OCW3_READ_REG | 0x00)

#define PIC_EOI			OCW2_EOI

extern void		pic_init();
extern void		pic_eoi(unsigned int irq);
extern int		pic_is_spurious_interrupt(unsigned int irq);
extern void		pic_enable_irq(unsigned int irq);
extern void		pic_disable_irq(unsigned int irq);

#endif /* ndef KERNEL_ARCH_PIC_H */
