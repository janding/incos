#include "arch/irq.h"
#include "arch/pic.h"
#include "kernel.h"
#include "sched.h"

extern void rtc_irq_handler();

void
handle_irq(uint32_t *esp, unsigned int irq)
{
	switch (irq) {
		case 8:
			rtc_irq_handler();
			break;
		default:
			kprintf("[irq%02d]\n", irq);
			break;
	}

	if (pic_is_spurious_interrupt(irq)) {
		kprintf("SPURIOUS!");
	}

	pic_eoi(irq);
}

void
enable_irq(unsigned int irq)
{
	pic_enable_irq(irq);
}

void
disable_irq(unsigned int irq)
{
	pic_disable_irq(irq);
}
