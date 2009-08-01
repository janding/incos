#include "arch/intr.h"
#include "kernel.h"
#include "sched.h"
#include "vm/vm.h"

#define IDLE		0x1D1E
#define RUNNABLE	1
#define SLEEPING	2

thread_t *first_thread, *last_thread;
__global
thread_t *current_thread;

thread_t idle_thread;

extern void arch_schedule(thread_t *current, thread_t *next);

extern int rtc_get_time();


/*__global __fastcall
void
schedule_fast(vaddr_t sp, vaddr_t cont)
{
	current_thread->stack_pointer = sp;
	current_thread->continuation = cont;
	//kprintf("current_thread[%08x} = %d\n", current_thread, current_thread->id);
	schedule();
	call_continuation(current_thread->stack_pointer, current_thread->continuation);
}*/

__global
void
schedule()
{
	thread_t *next_thread = current_thread;

	/*if (current_thread->ticks > 0) {
		current_thread->ticks--;
		return;
	} else {
		current_thread->ticks = current_thread->id;
	}*/

	do {
		if (next_thread == last_thread) {
			next_thread = first_thread;
		} else {
			next_thread = next_thread->next;
		}

		if (next_thread->state == SLEEPING) {
			if (rtc_get_time() > next_thread->sleep) {
				next_thread->state = RUNNABLE;
			}
		}

		if (next_thread->state != RUNNABLE && current_thread == next_thread) {
			next_thread = &idle_thread;
			break;
		}
	} while (next_thread->state != RUNNABLE);
	
	arch_schedule(current_thread, next_thread);
	
	current_thread = next_thread;
}

void
sched_first()
{
	arch_schedule(current_thread, current_thread);
}

void
sched_ready(thread_t *thread)
{
	thread->next = NULL;
	thread->state = RUNNABLE;
	thread->ticks = thread->id;
	last_thread->next = thread;
	last_thread = thread;
}

void
sched_kill(thread_t *thread)
{
	if (thread == current_thread)
		schedule();

	if (first_thread == thread)
		first_thread = thread->next;

	for (thread_t *t = first_thread; t; t = t->next) {
		if (t->next == thread) {
			t->next = thread->next;
			if (thread == last_thread)
				last_thread = t;
		}
	}
}

void
sched_sleep(thread_t *thread, int seconds)
{
	thread->state = SLEEPING;
	thread->sleep = rtc_get_time() + seconds * 1000;

	schedule();
}

void
sched_init()
{
	idle_thread.process = NULL;
	idle_thread.next = NULL;
	idle_thread.id = 0;
	idle_thread.state = IDLE;

	current_thread = &idle_thread;
	first_thread = &idle_thread;
	last_thread = &idle_thread;
}
