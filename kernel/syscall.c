#include "kernel.h"
#include "process.h"
#include "sched.h"
#include "syscall.h"

__fastcall
int
syscall_dprint(const char *s)
{
	return kprintf("%s", s);
}

__fastcall
int
syscall_gettid(void)
{
	return current_thread->id;
}

__fastcall
int
syscall_getpid(void)
{
	return current_thread->process->id;
}

__fastcall
void
syscall_exit(int e)
{
	sched_kill(current_thread);
	//free_thread(thread);
}

__fastcall
void
syscall_sleep(int seconds)
{
	//sched_sleep(current_thread, seconds);
	//free_thread(thread);
}
