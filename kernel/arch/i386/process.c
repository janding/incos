#include "process.h"
#include "vm/vm.h"
#include "vm/phys.h"

struct stack_frame
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, _esp, ebx, edx, ecx, eax;
    unsigned int interrupt, error;
    unsigned int eip, cs, eflags, esp, ss;
};

void
arch_process_map(process_t *process, vaddr_t va, paddr_t pa, size_t size, unsigned int flags)
{
	vm_pde_t * pgdir = (vm_pde_t*)0xfffff000;

	//if (pgdir[0x3fe] & 0xfffff000 != process->cr3) {
		pgdir[0x3fe] = process->cr3 | PDE_WRITE | PDE_PRESENT;
	
		invalidate_page(0xffffe000);
		invalidate_range(0xff800000, 0xffbfffff);
	//}

	map((vm_pde_t*)0xffffe000, (vm_pte_t*)0xff800000, va, pa, size, flags);

	invalidate_page((vaddr_t)process->pgdir_va);
}

void
arch_init_process(process_t *process)
{
	process->cr3 = phys_alloc_page();
	process->pgdir_va = vm_kmap_map(process->cr3, PAGE_SIZE);

	for (int i = 0; i < (KERNEL_BEGIN >> PGDIR_SHIFT); i++)
		process->pgdir_va[i] = 0;
	for (int i = (KERNEL_BEGIN >> PGDIR_SHIFT); i < 0x3fe; i++)
		process->pgdir_va[i] = ((vm_pde_t*)0xfffff000)[i];

	process->pgdir_va[0x3fe] = 0;
	process->pgdir_va[0x3ff] = process->cr3 | PDE_WRITE | PDE_PRESENT;
}

struct syscall_stack_frame
{
	unsigned int kesp;
	unsigned int eip, cs, eflags, esp, ss;
};

extern void syscall_return();

void
arch_init_thread(thread_t *thread, void *entry)
{
	thread->stack = (vaddr_t)vm_kmap_map(phys_alloc_zeroed_page(), PAGE_SIZE);
	struct syscall_stack_frame *sf = (struct syscall_stack_frame *)(thread->stack + PAGE_SIZE - sizeof(struct syscall_stack_frame));
	
	paddr_t user_stack = phys_alloc_zeroed_page();
	arch_process_map(thread->process, 0xef000000, user_stack, PAGE_SIZE, PG_U | PG_W);
	
	sf->ss = 0x20 | 3;
	sf->esp = 0xef000000 + PAGE_SIZE - 4;
	sf->eflags = 0x00202;
	sf->cs = 0x18 | 3;
	sf->eip = (uint32_t)entry;
	sf->kesp = (uint32_t)syscall_return;

	//thread->esp = (uint32_t*)sf;
	//*--thread->esp = (uint32_t)syscall_return;
	
	thread->stack_pointer = (uint32_t)sf;
	//thread->continuation = (uint32_t)syscall_return;
}
