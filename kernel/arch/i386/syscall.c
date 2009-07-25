#include "arch/idt.h"
#include "syscall.h"
#include "sched.h"

void do_syscall(uint32_t *esp, struct stack_frame *sf)
{
	switch (sf->eax)
	{
	case SYSCALL_EXIT:
		current_thread->esp = *esp;
		syscall_exit(sf->ebx);
		*esp = current_thread->esp;
		break;
	case SYSCALL_DPRINT:
		sf->eax = syscall_dprint((const char*)sf->ebx);
		break;
	case SYSCALL_GETPID:
		sf->eax = syscall_gettid();
		break;
	case SYSCALL_GETTID:
		sf->eax = syscall_gettid();
		break;
	}
}