#include "arch/cpu.h"
#include "arch/paging.h"
#include "arch/tss.h"
#include "kernel.h"
#include "sched.h"

#define DUMP_LIMIT		(vaddr_t)0xffbfffff

void
dump_page_table()
{
	vm_pde_t *pgdir = (vm_pde_t*)0xfffff000;
	vm_pte_t *pgtbl = (vm_pte_t*)0xffc00000;
	vaddr_t va, va_start;
	paddr_t pa, pa_start;
	int priv = -1;

	va = 0;
	pa = 0;
	va_start = 0;
	pa_start = 0;

	kprintf("cr3 = %08x\n", read_cr3());
	for (;;) {

		while (!(pgdir[va >> PGDIR_SHIFT] & PDE_PRESENT)) {
			if (va == 0xffc00000)
				break;
			if (va > DUMP_LIMIT)
				return;

			va += SUPERPAGE_SIZE;
		}

		while (!(pgdir[va >> PGDIR_SHIFT] & PDE_PRESENT)
			|| !(pgtbl[va >> PAGE_SHIFT] & PTE_PRESENT)) {
			
			if (va == 0xfffff000)
				break;
			if (va > DUMP_LIMIT)
				return;

			va += PAGE_SIZE;
		}

		if (va == 0xfffff000 || va > DUMP_LIMIT)
			break;

		va_start = va;
		pa_start = PAGE_TRUNC(pgtbl[va >> PAGE_SHIFT]);
		priv = pgtbl[va >> PAGE_SHIFT] & PTE_USER;

		while ((pgdir[va >> PGDIR_SHIFT] & PDE_PRESENT) 
			&& (pgtbl[va >> PAGE_SHIFT] & PTE_PRESENT)
			//&& (priv == pgtbl[va >> PAGE_SHIFT] & PDE_USER)
			&& (PAGE_TRUNC(pgtbl[va >> PAGE_SHIFT]) - pa_start == va - va_start)) {
			
			if (va == 0xfffff000 || va > DUMP_LIMIT)
				break;

			va += PAGE_SIZE;
		}
		
		kprintf("%08x-%08x -> %08x-%08x (%s)\n", va_start, va - 1, pa_start, pa_start + (va - 1 - va_start), priv == 0 ? "kernel" : "user");
		
		if (va == 0xfffff000 || va > DUMP_LIMIT)
			break;
	}
}

void
arch_schedule(thread_t *current, thread_t *next)
{
	if (next->process) {
		if (!current->process || current->process->cr3 != next->process->cr3) {
			for (int i = KERNEL_BEGIN >> PGDIR_SHIFT; i < 0x3ff; i++) {
				next->process->pgdir_va[i] = ((vm_pde_t*)0xfffff000)[i];
			}
			next->process->pgdir_va[0] = ((vm_pde_t*)0xfffff000)[0];
		}

		if (next->process->cr3 != 0) {
			write_cr3(next->process->cr3);

			//dump_page_table();
		}
		
		tss.esp0 = next->stack + PAGE_SIZE;
	}
}
