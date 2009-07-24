.text

.global _start
_start:
	# push parameters for vm_bootstrap
	push %ebx			# pointer to multiboot info
	push %eax			# multiboot magic

	# setup paging
	call vm_bootstrap
	
	# allocate stack
	call vm_phys_early_alloc_page
	add $4096, %eax

	# pop parameters and save them for init
	pop %edx
	pop %ebx
	
	# switch to new stack
	mov %eax, %esp
	
	# push parameters for init
	push %ebx			# pointer to multiboot info
	push %edx			# multiboot magic

	call init
	
1:	jmp 1b

