#include "kernel.h"
#include "process.h"
#include "sched.h"
#include "syscall.h"

int
syscall_dprint(const char *s)
{
	return kprintf("%s", s);
}

int
syscall_gettid(void)
{
	return current_thread->id;
}

int
syscall_getpid(void)
{
	return current_thread->process->id;
}

void
syscall_exit(int e)
{
	sched_kill(current_thread);
	//free_thread(thread);
}
