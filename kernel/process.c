#include "vm/vm.h"
#include "vm/phys.h"
#include "process.h"
#include "sched.h"

extern void	arch_init_process(process_t *process);
extern void	arch_init_thread(thread_t *thread, void *entry);
extern void	arch_process_map(process_t *process, vaddr_t va, paddr_t pa, size_t size, unsigned int flags);


static pmem_pool_t *process_pool;
static pmem_pool_t *thread_pool;

static uint32_t thread_id = 0;
static uint32_t process_id = 0;

void
process_map(process_t *process, vaddr_t va, paddr_t pa, size_t size)
{
	arch_process_map(process, va, pa, size, PG_U | PG_W);
}

void
process_zero_mem(process_t *process, vaddr_t va, size_t size)
{
	for (int i = 0; i < size; i += PAGE_SIZE)
		arch_process_map(process, va + i, phys_alloc_zeroed_page(), PAGE_SIZE, PG_U | PG_W);
}

process_t *
create_process()
{
	process_t *process = pmem_alloc(process_pool);
	process->id = ++process_id;

	arch_init_process(process);

	//process->thread = create_thread(process, entry);

	return process;
}

thread_t *
create_thread(process_t *process, void *entry)
{
	thread_t *thread = pmem_alloc(thread_pool);
	thread->id = ++thread_id;
	thread->process = process;

	arch_init_thread(thread, entry);

	sched_ready(thread);

	return thread;
}

void
proc_init()
{
	process_pool = pmem_create_pool(sizeof(process_t));
	thread_pool = pmem_create_pool(sizeof(thread_t));
}
