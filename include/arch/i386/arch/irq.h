#ifndef IRQ_H
#define IRQ_H

#include <types.h>

#define IRQ_BASE		0x20


void handle_irq(uint32_t *esp, int irq);

#endif /* ndef IRQ_H */
