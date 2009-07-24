#include "kernel.h"
#include "arch/paging.h"
#include "vm/vm.h"
#include "vm/phys.h"

void *
phys_to_virt(paddr_t pa, uint32_t low, uint32_t high)
{
	vm_pde_t *pgdir = (vm_pde_t*)0xfffff000;
	vm_pte_t *pgtbl = (vm_pte_t*)0xffc00000;
	uint32_t offset = pa - PAGE_ADDRESS(pa);
	pa = PAGE_ADDRESS(pa);

	uint32_t i = PAGE_TRUNC(low) >> PAGE_SHIFT; 
	while (i <= PAGE_ROUND(high) >> PAGE_SHIFT) {
		if (!(pgdir[(i << PAGE_SHIFT) >> PGDIR_SHIFT] & PDE_PRESENT)) {
			i += ENTRIES_PER_PAGE;
			continue;
		}

		if ((pgtbl[i] & PTE_PRESENT) && (PAGE_ADDRESS(pgtbl[i]) == pa)) {
			return (void*)((i << PAGE_SHIFT) + offset);
		}

		i++;
	}

	return 0;
}

void
invalidate_page(vaddr_t addr)
{
	__asm__ __volatile__ ("invlpg %0" : : "m"(*(char*)addr) : "memory");
}

void
invalidate_range(vaddr_t from, vaddr_t to)
{
	for (vaddr_t addr = from; addr < to; addr += PAGE_SIZE)
		invalidate_page(addr);
}


void
map(vm_pde_t *pgdir_va, vm_pte_t *pgtbl_va, vaddr_t va, paddr_t pa, size_t size)
{
	vaddr_t va_a = va, va_e = va + size - 1;

	while (size > 0) {
		if (!(pgdir_va[va >> PGDIR_SHIFT] & PDE_PRESENT)) {

			if ((size >= SUPERPAGE_SIZE) && ((va & SUPERPAGE_MASK) == 0)/* && (cpu_features & CPU_PSE)*/) {
				// 4 MB page
				pgdir_va[va >> PGDIR_SHIFT] = pa | PDE_PAGE_SIZE | PDE_USER | PDE_WRITE | PDE_PRESENT;
				
				va += SUPERPAGE_SIZE;
				pa += SUPERPAGE_SIZE;
				size -= SUPERPAGE_SIZE;
				continue;
			}

			pgdir_va[va >> PGDIR_SHIFT] = phys_alloc_zeroed_page(PAGE_SIZE)
				| PDE_USER | PDE_WRITE | PDE_PRESENT;
			//kprintf("allocated pgdir_va %08x[%08x] = %08x for %08x\n", pgdir_va, va >> PGDIR_SHIFT, pgdir_va[va >> PGDIR_SHIFT], va & 0xffc00000);
			
			if (pgtbl_va != NULL)
				invalidate_page((vaddr_t)(pgtbl_va + (va >> PAGE_SHIFT)));
		}
		
		if (pgtbl_va != NULL)
			pgtbl_va[va >> PAGE_SHIFT] = pa | PTE_USER | PTE_WRITE | PTE_PRESENT;
		else
			((vm_pte_t*)(pgdir_va[va >> PGDIR_SHIFT] & ~PAGE_MASK))[(va >> PAGE_SHIFT) % ENTRIES_PER_PAGE] = pa | PDE_USER | PTE_WRITE | PTE_PRESENT;

		//kprintf("mapped pgtbl_va %08x[%08x] = %08x\n",  pgtbl_va, (va >> PAGE_SHIFT), pa);

		va += PAGE_SIZE;
		pa += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	invalidate_range(va_a, va_e);
}


void *
map_range_into_kernel(paddr_t pa, size_t size)
{
	vm_pde_t *pgdir = (vm_pde_t*)0xfffff000;
	vm_pte_t *pgtbl = (vm_pte_t*)0xffc00000;
	int contiguous;

	contiguous = 1;
	for (int i = 0; i < PAGE_ROUND(size); i += PAGE_SIZE) {
		if (phys_to_virt(pa + KERNEL_BEGIN + i, KERNEL_BEGIN, KERNEL_END) != 0) {
			contiguous = 0;
			break;
		}
	}

	if (contiguous) {
		map(pgdir, pgtbl, pa + KERNEL_BEGIN, pa, PAGE_ROUND(size));
		return (void*)(pa + KERNEL_BEGIN);
	}

	panic("map_range_into_kernel: failed");
}

void *
map_into_kernel(paddr_t pa)
{
	
	return map_range_into_kernel(pa, PAGE_SIZE);
	
	/*vm_pde_t *pgdir = (vm_pde_t*)0xfffff000;
	vm_pte_t *pgtbl = (vm_pte_t*)0xffc00000;

	if (phys_to_virt(pa + KERNEL_BEGIN, KERNEL_BEGIN, KERNEL_END) == 0) {
		map(pgdir, pgtbl, pa + KERNEL_BEGIN, pa, PAGE_SIZE);
		return (void*)(pa + KERNEL_BEGIN);
	}

	panic("map_into_kernel: failed");*/
}