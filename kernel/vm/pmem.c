/* 
 * Allocator for physical memory.
 *
 * This allocator is similar to a slab allocator. 
 */

#include <stddef.h>

#include "kernel.h"

#include "vm/vm.h"
#include "vm/phys.h"

/* 
 * A pool is a single physical page containing objects. Small elements of 
 * fixed size (less than 2000 bytes) can be allocated from this page. If a 
 * page is full, allocations are served from the next empty or partially full 
 * page in a linked list. If there is no page with free slots in this list a 
 * new page will be allocated and appended to this list.
 */

struct pmem_pool
{
	/* linked list of additional pools of the same type */
	pmem_pool_t *next;
	/* number of free slots in this page */
	unsigned int capacity;
	/* size of an element */
	unsigned int elsize;
	/* pointer to first free slot. NULL if pool has no gaps. */
	void **free;


	void *dummy;
};

/* number of elements size a pool can store */
#define PMEM_POOL_CAPACITY(elsize) ((PAGE_SIZE - offsetof(pmem_pool_t, dummy)) / (elsize))

/* keeps track of all allocated pools */
static pmem_pool_t **pools;
static int used_pools;

void
pmem_init()
{
	pools = (pmem_pool_t**)vm_kmap_map(phys_alloc_page(), PAGE_SIZE);
	used_pools = 0;
}

pmem_pool_t *
pmem_create_pool(unsigned int elsize)
{
	pmem_pool_t *pool;
	
	pools[used_pools] = (pmem_pool_t*)vm_kmap_map(phys_alloc_page(), PAGE_SIZE);
	pool = pools[used_pools];
	used_pools++;

	if (elsize < 4) elsize = 4;
	if (elsize > 2000) panic("vm_pmem_create_pool: elsize > 2000");

	pool->next = NULL;
	pool->capacity = PMEM_POOL_CAPACITY(elsize);
	pool->elsize = elsize;
	pool->free = NULL;

	return pool;
}

void *
pmem_alloc(pmem_pool_t *pool)
{
	void *result;

	while (pool->capacity == 0 && pool->next) {
		pool = pool->next;
	}

	if (pool->capacity == 0) {
		pool->next = pmem_create_pool(pool->elsize);
		pool = pool->next;
	}

	if (pool->free == NULL) {
		result = (void*)((char*)pool + PAGE_SIZE - pool->capacity * pool->elsize);
	} else {
		result = pool->free;
		pool->free = *pool->free;
	}
	
	pool->capacity--;
	
	return result;
}

void
pmem_free(pmem_pool_t *pool, void *p)
{
	for ( ; pool; pool = pool->next) {
		if (((paddr_t)pool & 0xfffff000) == ((paddr_t)p & 0xfffff000))
			break;
	}

	if (pool == NULL) {
		panic("vm_pmem_free: object not in pool");
	}

	/* todo: handle special case if p was last element in page? */

	/* add slot to front of list */
	*(void**)p = pool->free;
	pool->free = (void**)p;
	pool->capacity++;
}
