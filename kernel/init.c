#include "kernel.h"
#include "arch/intr.h"
#include "process.h"

extern void vm_init();
extern void proc_init();
extern void sched_init();
extern void run_boot_modules();

void
kern_init()
{
	kprintf("Loading IncOS ...\n");

	vm_init();
	proc_init();
	sched_init();

	run_boot_modules();

	enable_interrupts();

	for (;;);
}

