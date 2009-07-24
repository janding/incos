#ifndef ARCH_VM_H
#define ARCH_VM_H

#include "arch/paging.h"
#include "types.h"

#define KERNEL_BEGIN		0xf0000000
#define KERNEL_END			0xffc00000

/*#define VM_KERNEL_BASE		0xf0000000
#define VM_KERNEL_BASE_PDE	(0xf0000000 >> PGDIR_SHIFT)

struct vm_space
{
	paddr_t pgdir_pa;
};

typedef struct vm_space *vm_space_t;

extern struct vm_space kernel_space_storage;
#define kernel_space (&kernel_space_storage)

extern void arch_vm_init();*/

#endif /* ndef ARCH_VM_H */
