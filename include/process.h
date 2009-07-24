#ifndef PROCESS_H
#define PROCESS_H

#include "arch/process.h"

extern process_t *	create_process();
extern thread_t *	create_thread(process_t *process, void *entry);
extern void			process_map(process_t *process, vaddr_t va, paddr_t pa, size_t size);
extern void			process_zero_mem(process_t *process, vaddr_t va, size_t size);

#endif /* ndef PROCESS_H */
