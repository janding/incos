.global _start
_start:
	xor %ebp, %ebp
	push $0xef800000
	call _main
	
	# syscall_exit
	xor %eax, %eax
	xor %ebx, %ebx
	int $0x30
