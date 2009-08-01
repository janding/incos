#ifndef IRQ_H
#define IRQ_H

#include <types.h>

#define IRQ_BASE		0x20


extern void	handle_irq(uint32_t *esp, unsigned int irq);
extern void	enable_irq(unsigned int irq);
extern void	disable_irq(unsigned int irq);

#endif /* ndef IRQ_H */
