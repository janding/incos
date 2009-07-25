#include "arch/idt.h"
#include "syscall.h"

void do_syscall(uint32_t *esp, struct stack_frame *sf)
{
	switch (sf->eax)
	{
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