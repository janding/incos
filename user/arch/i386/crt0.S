.global _start
_start:
	xor %ebp, %ebp
	call main
	call _exit

_exit:
	jmp .
