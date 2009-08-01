#include <types.h>

#include "kernel.h"

#include "vm/vm.h"

__global
paddr_t phys_sbrk_init;
__global
paddr_t phys_sbrk;
__global
paddr_t phys_space_end;

static unsigned int early_alloc_allowed = 1;

paddr_t
vm_phys_early_alloc_pages(unsigned int num)
{
	if (!early_alloc_allowed) panic("vm_phys_early_alloc_page() not allowed after vm_num_boot_pages()");

	paddr_t r = phys_sbrk;
	phys_sbrk += PAGE_SIZE * num;
	return r;
}

__global
paddr_t
vm_phys_early_alloc_page()
{
	return vm_phys_early_alloc_pages(1);
}

unsigned int
vm_num_phys_pages()
{
	return phys_space_end / PAGE_SIZE;
}

unsigned int
vm_num_boot_pages()
{
	early_alloc_allowed = 0;

	return phys_sbrk / PAGE_SIZE;
}

unsigned int
vm_num_static_pages()
{
	return phys_sbrk_init / PAGE_SIZE;
}

void
arch_vm_init()
{
}
