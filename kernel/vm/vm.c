#include <stddef.h>

#include "kernel.h"

#include "vm/vm.h"
#include "vm/phys.h"

extern void arch_vm_init();
extern void phys_init();
extern void pmem_init();
extern void pmem_to_kmap();
extern void kmem_init();

extern char kernel_phys_start;
extern char kernel_phys_end;
extern char kernel_virt_start;

typedef struct vm_kpage vm_kpage_t;

struct vm_kpage
{
	vm_kpage_t *next;

	paddr_t pa;
	vaddr_t va;
};

typedef struct vm_object vm_object_t;

struct vm_object
{
	vm_kpage_t *pages;

	const char *name;
};


static pmem_pool_t *kpage_pool;
static pmem_pool_t *object_pool;

static vm_object_t *kernel_object = NULL;
static paddr_t **early_alloc_list = NULL;


void
vm_kmap_add_page_range(vm_object_t *object, paddr_t pa, vaddr_t va, uint32_t size)
{
	for (uint32_t offset = 0; offset < size; offset += PAGE_SIZE) {
		vm_kpage_t *page = pmem_alloc(kpage_pool);
		page->next = object->pages;
		page->pa = pa + offset;
		page->va = va + offset;
		object->pages = page;
	}
}


vm_object_t *
vm_kmap_create_object(const char *name)
{
	vm_object_t *object;

	object = pmem_alloc(object_pool);
	object->name = name;
	object->pages = NULL;


	return object;
}

void
_kmap_dump(vm_object_t *object)
{
	kprintf("object dump %08x", object);
	kprintf(" (%s)\n", object->name);
	kprintf("pages %08x\n", object->pages);
	for (vm_kpage_t *page = object->pages; page; page = page->next) {
		kprintf("%08x -> %08x\n", page->va, page->pa);
	}
}

void
vm_kmap_init()
{
	kpage_pool = pmem_create_pool(sizeof(vm_kpage_t));
	object_pool = pmem_create_pool(sizeof(vm_object_t));

	vm_object_t *t_kern_obj = vm_kmap_create_object("kernel");

	// register mapping for kernel binary
	vm_kmap_add_page_range(t_kern_obj, 
		PAGE_TRUNC((paddr_t)&kernel_phys_start), PAGE_TRUNC((vaddr_t)&kernel_virt_start), 
		PAGE_ROUND((uint32_t)&kernel_phys_end) - PAGE_TRUNC((uint32_t)&kernel_phys_start));
	
	// map page tables
	vm_pte_t *pgdir = (vm_pde_t*)0xfffff000;
	vm_pte_t *pgtbl = (vm_pte_t*)0xffc00000;
	for (int i = 0; i < ENTRIES_PER_PAGE; i++) {
		if (pgdir[i] & PDE_PRESENT) {
			vm_kmap_add_page_range(t_kern_obj, PAGE_TRUNC(pgdir[i]), ((vaddr_t)pgtbl) + (i << PAGE_SHIFT), PAGE_SIZE);
		}
	}

	// map pages allocated before this function was called
	for (paddr_t *p = (paddr_t*)&early_alloc_list[1]; p <= early_alloc_list[0]; p++) {
		vm_kmap_add_page_range(t_kern_obj, *p, *p + KERNEL_BEGIN, PAGE_SIZE);
	}

	phys_free_page((paddr_t)early_alloc_list);
	early_alloc_list = NULL;

	kernel_object = t_kern_obj;
}

/*void *
vm_kmap_alloc_pages(size_t num)
{
	
}*/

void *
vm_kmap_map(paddr_t pa, size_t size)
{
	if (kernel_object == NULL) {
		for (int i = 0; i < size; i += PAGE_SIZE) {
			*++*early_alloc_list = pa + i;
		}
		return (void*)(pa + KERNEL_BEGIN);
	}

	vm_kmap_add_page_range(kernel_object, pa, pa + KERNEL_BEGIN, size);
	//map(va, pa, size);
	return (void*)(pa + KERNEL_BEGIN);
	//panic("panik!");
	//return vm_kmap_map(kernel_object, pa, size);
}

void
vm_init()
{
	arch_vm_init();

	early_alloc_list = (paddr_t **)vm_phys_early_alloc_page();
	early_alloc_list[0] = (paddr_t *)early_alloc_list;

	phys_init();
	pmem_init();

	vm_kmap_init();

	//_kmap_dump(kernel_object);

	kmem_init();
}
