.macro interrupt_stub num
.global interrupt_stub_\()\num
interrupt_stub_\()\num:
.ifne \num-8
.ifne \num-10
.ifne \num-11
.ifne \num-12
.ifne \num-13
.ifne \num-14
.ifne \num-17
	pushl $0
.endif
.endif
.endif
.endif
.endif
.endif
.endif
	pushl $\num
	jmp isr_common
.endm

.macro interrupt_stub_error_code num
.global interrupt_stub_\()\num
interrupt_stub_\()\num:
	pushl $\num
	jmp isr_common
.endm

# Exceptions
interrupt_stub 0
interrupt_stub 1
interrupt_stub 2
interrupt_stub 3
interrupt_stub 4
interrupt_stub 5
interrupt_stub 6
interrupt_stub 7
interrupt_stub_error_code 8
interrupt_stub 9
interrupt_stub_error_code 10
interrupt_stub_error_code 11
interrupt_stub_error_code 12
interrupt_stub_error_code 13
interrupt_stub_error_code 14
interrupt_stub 16
interrupt_stub_error_code 17
interrupt_stub 18
interrupt_stub 19
 
# IRQs (vorausgesetzt diese sind nach 0x20 bis 0x2f gemappt)
interrupt_stub 32
interrupt_stub 33
interrupt_stub 34
interrupt_stub 35
interrupt_stub 36
interrupt_stub 37
interrupt_stub 38
interrupt_stub 39
interrupt_stub 40
interrupt_stub 41
interrupt_stub 42
interrupt_stub 43
interrupt_stub 44
interrupt_stub 45
interrupt_stub 46
interrupt_stub 47
 
# Systemaufruf
interrupt_stub 48

.global irq0_handler
irq0_handler:
	push %eax
	push %ecx
	push %edx
	push %ebx
	push %ebp
	push %esi
	push %edi
	
	mov $0x10, %ax
	mov %ax, %ds

	mov current_thread, %ebx
	
	call schedule
	
	mov current_thread, %eax
	
	# if (eax == ebx) { out 0x20, 0x20; goto irq_return }
	
	push $irq_return
	mov %esp, 0(%ebx)
	mov 0(%eax), %esp
	pop %ecx
	mov $0x20, %al
	out %al, $0x20
	jmp *%ecx
	
irq_return:
	mov $0x23, %ax
	mov %ax, %ds
	pop %edi
	pop %esi
	pop %ebp
	pop %ebx
	pop %edx
	pop %ecx
	pop %eax
	iret


.global syscall_handler
syscall_handler:
	mov $0x10, %bx
	mov %bx, %ds
	
	mov syscalls(,%eax,4), %eax
	call *%eax
	
	//mov %esp, %ecx
	//mov $syscall_return, %edx
	//jmp schedule_fast

.global syscall_return
syscall_return:
	mov $0x23, %bx
	mov %bx, %ds
	iret


.global syscall_rdtsc_handler
syscall_rdtsc_handler:
#	mov $0x10, %bx
#	mov %bx, %ds
	
	rdtsc

#	mov $0x23, %bx
#	mov %bx, %ds
	iret

isr_common:	
	# Alle Register sichern
	pusha
	push %ds
	push %es
	push %fs
	push %gs

	# In die Datensegmente einen Kernelselektor laden, damit wir 
	# garantiert mit Kernelprivilegien auf den Speicher zugreifen
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	# ESP �bergeben
	push %esp
	# Aufruf der eigentlichen Behandlung
	call int_handler
	# R�ckgabewert in EAX nach ESP schreiben
	# -> Stack Wechsel geschieht hier
	mov %eax, %esp

	# Alte Registerwerte in umgekehrter Reihenfolge wiederherstellen
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa

	# Fehlercode und Interruptnummer vom Stack holen
	add $8, %esp

	# R�cksprung
	iret
