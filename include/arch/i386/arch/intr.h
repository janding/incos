#ifndef ARCH_INTR_H
#define ARCH_INTR_H

#define enable_interrupts() __asm__ __volatile__ ("sti")
#define disable_interrupts() __asm__ __volatile__ ("cli")

extern void syscall_return();

#define call_continuation(sp, continuation) \
		__asm__ __volatile__("mov %0, %%esp; jmp *%1" :: "r"(sp), "r"(continuation)); __builtin_trap();

#endif /* ndef ARCH_INTR_H */
