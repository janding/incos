#include "kernel.h"
#include "arch/intr.h"
#include "process.h"

extern void vm_init();
extern void proc_init();
extern void sched_init();
extern void run_boot_modules();

void
bar()
{
	for (;;)
		kprintf("bar");
}

void
foo()
{
	for (;;)
		kprintf("foo");
}

void
kern_init()
{
	kprintf("Loading IncOS ...\n");

	vm_init();
	proc_init();
	sched_init();

	process_t * p1 = create_process();
	create_thread(p1, foo);
	process_t * p2 = create_process();
	create_thread(p2, bar);

	run_boot_modules();

	enable_interrupts();

	for (;;)
		kprintf("i");
}

