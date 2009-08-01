#include "arch/idt.h"
#include "arch/intr.h"
#include "kernel.h"
#include "syscall.h"
#include "sched.h"

__global
void *syscalls[] = {
	[SYSCALL_EXIT] syscall_exit,
	[SYSCALL_DPRINT] syscall_dprint,
	[SYSCALL_GETPID] syscall_gettid,
	[SYSCALL_GETTID] syscall_gettid,
	[SYSCALL_SLEEP] syscall_sleep,
};


struct regs { unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; };
#define CMP_REG(reg) do { if (good.reg != bad.reg) kprintf("[REG " #reg " %08x - %08x]", good.reg, bad.reg); } while(0)
void regdiff(struct regs bad, struct regs good) { 
	CMP_REG(eax);
	CMP_REG(ecx);
	CMP_REG(edx);
	CMP_REG(ebx);
	//CMP_REG(esp);
	CMP_REG(ebp);
	CMP_REG(esi);
	CMP_REG(edi);
}	

/*
int do_syscall(int eax, int param1);

__global __fastcall
void
syscall(vaddr_t sp, vaddr_t cont, int dummy1, int dummy2, int dummy3, int dummy4, int dummy5, int param1)
{
	//int eax;
	//__asm__("mov %%eax, %0\n\t" : "=r"(eax));

	current_thread->stack_pointer = sp;
	current_thread->continuation = cont;

	//kprintf("syscall %d %x %x %x %x %x %d\n", sp, cont, eax, dummy1, dummy2, dummy3, dummy4, dummy5, param1);
	do_syscall(eax, param1);

	call_continuation(current_thread->stack_pointer, current_thread->continuation);
}

int
do_syscall(int number, int param1)
{
	switch (number)
	{
	case SYSCALL_EXIT:
		syscall_exit(param1);
		break;
	case SYSCALL_DPRINT:
		return syscall_dprint((const char*)param1);
		break;
	case SYSCALL_GETPID:
		return syscall_gettid();
		break;
	case SYSCALL_GETTID:
		return syscall_gettid();
		break;
	case SYSCALL_SLEEP:
		syscall_sleep(param1);
		break;
	}
}
*/