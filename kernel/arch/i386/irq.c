#include "arch/irq.h"
#include "arch/pic.h"
#include "kernel.h"
#include "sched.h"

extern void rtc_irq_handler();

void
handle_irq(uint32_t *esp, int irq)
{
	switch (irq) {
		case 0:
			current_thread->esp = (uint32_t*)*esp;
			schedule();
			*esp = (uint32_t)current_thread->esp;
			break;
		case 8:
			rtc_irq_handler();
		default:
			kprintf("[irq%02d]\n", irq);
			break;
	}

	if (pic_is_spurious_interrupt(irq)) {
		kprintf("SPURIOUS!");
	}

	pic_eoi(irq);
}
