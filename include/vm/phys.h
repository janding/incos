#ifndef PHYS_H
#define PHYS_H

typedef paddr_t phys_page_t;

typedef struct stack_page stack_page_t;
struct stack_page {
	unsigned int entries;
	stack_page_t *next;
	phys_page_t pages[];
};

extern stack_page_t *phys_pages;
extern unsigned int *phys_bitmap;
extern unsigned int phys_bitmap_size;

extern void		phys_free_page(paddr_t pa);
extern paddr_t	phys_alloc_page();
extern paddr_t	phys_alloc_zeroed_page();

#endif /* ndef PHYS_H */
