#ifndef ARCH_PROCESS_H
#define ARCH_PROCESS_H

#include <types.h>
#include "vm/vm.h"

typedef struct process process_t;
typedef struct thread thread_t;

struct process
{
	thread_t *thread;
	uint32_t id;

	vm_pde_t *pgdir_va;
	paddr_t cr3;
};

struct thread
{
	vaddr_t stack_pointer;
	vaddr_t continuation;

	process_t *process;
	uint32_t id;
	thread_t *next;
	int state;
	int sleep;
	uint32_t ticks;

	vaddr_t stack;
};

#endif /* ndef ARCH_PROCESS_H */
