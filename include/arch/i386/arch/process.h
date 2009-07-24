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
	process_t *process;
	uint32_t id;
	thread_t *next;

	uint32_t *esp;
	vaddr_t stack;
};

#endif /* ndef ARCH_PROCESS_H */
