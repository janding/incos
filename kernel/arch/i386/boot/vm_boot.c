#include "arch/cpu.h"
#include "arch/paging.h"
#include "arch/vm/vm.h"

#include "multiboot.h"

static vm_pde_t *boot_pgdir;

extern char kernel_phys_start;
extern char kernel_phys_end;
extern char kernel_virt_start;
extern char kernel_virt_end;

extern paddr_t phys_sbrk_init;
extern paddr_t phys_sbrk;
extern paddr_t phys_space_end;

static void *
vm_boot_alloc_page(paddr_t *pa)
{
	void *r = (void*)*pa;
	*pa += PAGE_SIZE;
	return r;
}

static paddr_t
vm_bootstrap_phys(multiboot_info_t *mb_info)
{
	paddr_t mem_end = 0;

	if (mb_info->flags & MULTIBOOT_MMAP) {
		// iterate over memory map
		for (multiboot_mmap_t *mmap = (multiboot_mmap_t*)mb_info->mmap_addr;
			(paddr_t)mmap < mb_info->mmap_addr + mb_info->mmap_length;
			mmap = (multiboot_mmap_t*)((paddr_t)mmap + mmap->size + sizeof(mmap->size))) {
			
			if (mmap->type != 1)
				continue;

			if (mem_end < mmap->base_addr + mmap->length)
				mem_end = mmap->base_addr + mmap->length;
		}
	}
	else if (mb_info->flags & MULTIBOOT_MEM) {
		if (mem_end < 0x100000 + (mb_info->mem_upper << 10))
			mem_end = 0x100000 + (mb_info->mem_upper << 10);
	}

	return mem_end;
}

static paddr_t
vm_sbrk_init(multiboot_info_t *mb_info)
{
	paddr_t sbrk = (paddr_t)&kernel_phys_end;
	multiboot_module_t *modules;

	if (mb_info->flags & MULTIBOOT_MODULES) {
		modules = (multiboot_module_t *)mb_info->mods_addr;
		for (int i = 0; i < mb_info->mods_count; i++) {
			if (sbrk < modules[i].mod_end)
				sbrk = modules[i].mod_end;
		}
	}

	return PAGE_ROUND(sbrk);
}

void 
vm_bootstrap(uint32_t mb_magic, multiboot_info_t *mb_info)
{
	paddr_t mem_end, sbrk, identity_map_end;
	paddr_t phys_start;
	vaddr_t virt_start, virt_end, vtop;
	
	mem_end = vm_bootstrap_phys(mb_info);
	sbrk = vm_sbrk_init(mb_info);

	boot_pgdir = vm_boot_alloc_page(&sbrk);

	// setup identity mapping for up to 3.75 GB
	identity_map_end = mem_end < KERNEL_BEGIN ? mem_end : KERNEL_BEGIN;

	for (unsigned int i = 0; i < SUPERPAGE_ROUND(identity_map_end); i += SUPERPAGE_SIZE) {
		vm_pte_t *pgtbl = vm_boot_alloc_page(&sbrk);
		
		boot_pgdir[i >> PGDIR_SHIFT] = (vm_pde_t)(pgtbl) | PDE_WRITE | PDE_PRESENT;
		
		for (unsigned int j = 0; j < ENTRIES_PER_PAGE; j++) {
			pgtbl[j] = (vm_pte_t)(i + (j << PAGE_SHIFT)) | PTE_WRITE | PTE_PRESENT;
		}
	}

	for (unsigned int i = SUPERPAGE_ROUND(identity_map_end); i < KERNEL_END; i += SUPERPAGE_SIZE) {
		boot_pgdir[i >> PGDIR_SHIFT] = 0;
	}

	// recursive page directory mapping
	boot_pgdir[0x3ff] = (vm_pde_t)boot_pgdir | PDE_PRESENT;

	phys_start = PAGE_TRUNC((paddr_t)&kernel_phys_start);
	virt_start = PAGE_TRUNC((vaddr_t)&kernel_virt_start);
	virt_end = PAGE_ROUND((vaddr_t)&kernel_virt_end);

	// virtual address + _vtop = physical address
	vtop = phys_start - virt_start;

	// map kernel
	for (unsigned int i = SUPERPAGE_TRUNC(virt_start); i < SUPERPAGE_ROUND(virt_end); i += SUPERPAGE_SIZE) {
		vm_pte_t *pgtbl = vm_boot_alloc_page(&sbrk);
		
		boot_pgdir[i >> PGDIR_SHIFT] = (vm_pde_t)(pgtbl) | PDE_WRITE | PDE_PRESENT;
		
		for (unsigned int j = 0; j < ENTRIES_PER_PAGE; j++) {
			pgtbl[j] = (vm_pte_t)(i + (j << PAGE_SHIFT) + vtop) | PTE_WRITE | PTE_PRESENT;
		}
	}

	// set page directory
	write_cr3((paddr_t)boot_pgdir);

	// enable paging
	write_cr0(read_cr0() | 0x80000000);

	phys_sbrk_init = sbrk;
	phys_sbrk = sbrk;
	phys_space_end = mem_end;
}
