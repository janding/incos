/* allocator for physical pages */

#include <stddef.h>

#include "arch/cpu.h"
#include "kernel.h"

#include "vm/vm.h"
#include "vm/phys.h"

#define STACK_PAGE_CAPACITY ((PAGE_SIZE - offsetof(stack_page_t, pages)) / sizeof(phys_page_t))

stack_page_t *phys_pages;
unsigned int *phys_bitmap;
unsigned int phys_bitmap_size;

unsigned int 
bit_scan_forward(unsigned int a)
{
	unsigned int result;
	__asm__("bsfl %1, %0\n\t" : "=r"(result) : "r"(a));
	return result;
}

// do not call vm_kmap_map() from this function
paddr_t
phys_alloc_page()
{
	paddr_t result = 0;
	unsigned int idx;

	if (phys_pages->entries == 0) {
		if (phys_pages->next != NULL) {
			/* reuse stack page */
			result = (paddr_t)phys_pages;
			phys_pages = phys_pages->next;
		} else {
			/* scan in bitmap */
			for (int i = 0; i < phys_bitmap_size; i++) {
				if (phys_bitmap[i] != 0) {
					result = (i * 32 + bit_scan_forward(phys_bitmap[i])) << PAGE_SHIFT;
					break;
				}
			}
		}
	} else {
		/* found page in phys_pages */
		result = phys_pages->pages[--phys_pages->entries];
	}

	if (result == 0)
		panic("vm_phys_alloc_page: out of memory");

	/* mark page as used */
	idx = result >> PAGE_SHIFT;
	phys_bitmap[idx / 32] &= ~(1 << (idx % 32));

	return result;
}

paddr_t
phys_alloc_zeroed_page()
{
	paddr_t result = phys_alloc_page();
	char *p = vm_kmap_map_temporary(result);

	for (int i = 0; i < PAGE_SIZE; i++)
		p[i] = 0;

	vm_kmap_unmap_temporary(result);
	return result;
}

void
phys_free_page(paddr_t pa)
{
	stack_page_t *p;
	unsigned int idx;

	if (pa == 0)
		panic("vm_phys_free_page(0)");

	if (phys_pages->entries == STACK_PAGE_CAPACITY) {
		/* stack page is full -> use pa as new stack page instead of marking it as free */
		p = (stack_page_t*)vm_kmap_map(pa, PAGE_SIZE);
		p->entries = 0;
		p->next = phys_pages; 
		phys_pages = p;
		return;
	}

	phys_pages->pages[phys_pages->entries++] = pa;

	idx = pa >> PAGE_SHIFT;
	phys_bitmap[idx / 32] |= (1 << (idx % 32));
}

void
phys_init()
{
	unsigned int pages;
	
	phys_pages = vm_kmap_map(vm_phys_early_alloc_page(), PAGE_SIZE);
	phys_pages->entries = 0;
	phys_pages->next = NULL;

	pages = vm_num_phys_pages();
	phys_bitmap = vm_kmap_map(vm_phys_early_alloc_pages(PAGE_ROUND(pages) / PAGE_SIZE), PAGE_ROUND(pages));
	phys_bitmap_size = (pages + 31) / 32;

	for (unsigned int i = 0; i < (vm_num_boot_pages() + 31) / 32; i++) {
		if (i < vm_num_boot_pages() / 32)
			phys_bitmap[i] = 0;
		else
			phys_bitmap[i] = ~0 << (vm_num_boot_pages() % 32);
	}
	for (unsigned int i = (vm_num_boot_pages() + 31) / 32; i < phys_bitmap_size; i++) {
		phys_bitmap[i] = ~0;
	}
}
