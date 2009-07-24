#ifndef PMEM_H
#define PMEM_H

typedef struct pmem_pool pmem_pool_t;

extern void			 pmem_init();
extern pmem_pool_t	*pmem_create_pool(unsigned int elsize);
extern void			*pmem_alloc(pmem_pool_t *pool);
extern void			 pmem_free(pmem_pool_t *pool, void *p);

#endif /* ndef PMEM_H */
