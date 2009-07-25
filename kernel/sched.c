#include "kernel.h"
#include "sched.h"

thread_t *first_thread, *last_thread;
thread_t *current_thread;

thread_t idle_thread;

extern void arch_schedule(thread_t *current, thread_t *next);

void
schedule()
{
	thread_t *next_thread;

	if (current_thread == last_thread) {
		next_thread = first_thread;
	} else {
		next_thread = current_thread->next;
	}
	
	arch_schedule(current_thread, next_thread);
	
	current_thread = next_thread;
}

void
sched_ready(thread_t *thread)
{
	thread->next = NULL;
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
sched_init()
{
	idle_thread.process = NULL;
	idle_thread.next = NULL;
	idle_thread.id = 0;

	current_thread = &idle_thread;
	first_thread = &idle_thread;
	last_thread = &idle_thread;
}
