#ifndef VM_H
#define VM_H

#include <types.h>

#include "arch/vm/vm.h"
#include "vm/pmem.h"


extern void *	vm_kmap_map(paddr_t pa, size_t size);
#define vm_kmap_map_temporary(p) vm_kmap_map(p, PAGE_SIZE)
#define vm_kmap_unmap_temporary(p)

// arch/vm/vm.c
extern paddr_t	vm_phys_early_alloc_page();
extern paddr_t	vm_phys_early_alloc_pages(unsigned int num);

extern unsigned int	vm_num_phys_pages();
extern unsigned int	vm_num_boot_pages();
extern unsigned int	vm_num_static_pages();

#define PG_W		2
#define PG_U		4

// arch/vm/vm_page.c
//extern void *	virt_to_phys(paddr_t pa, unsigned int low, unsigned int high);
extern void *	phys_to_virt(paddr_t pa, unsigned int low, unsigned int high);
extern void		invalidate_page(vaddr_t addr);
extern void		invalidate_range(vaddr_t from, vaddr_t to);
extern void		map(vm_pde_t *pgdir_va, vm_pte_t *pgtbl_va, vaddr_t va, paddr_t pa, size_t size, unsigned int flags);
extern void *	map_into_kernel(paddr_t pa);


// kernel/vm/vm_phys.c


#endif /* ndef VM_H */
