#ifndef SYSCALL_H
#define SYSCALL_H

#include "syscalls.h"

#define syscall0(number) ({ \
		int result; \
		__asm__ __volatile__("int $0x30\n\t" : "=a"(result) : "a"(number)); \
		result; \
	})

#define syscall1(number, param1) ({ \
		int result; \
		__asm__ __volatile__("int $0x30\n\t" : "=a"(result) : "a"(number), "b"(param1)); \
		result; \
	})

static inline int
syscall_dprint(const char *f)
{
	return syscall1(SYSCALL_DPRINT, f);
}

static inline int
syscall_getpid(void)
{
	return syscall0(SYSCALL_GETPID);
}

static inline int
syscall_gettid(void)
{
	return syscall0(SYSCALL_GETTID);
}

__attribute__((__noreturn__))
static inline void
syscall_exit(int code)
{
	syscall1(SYSCALL_EXIT, code);
	for (;;);
}

#endif /* ndef SYSCALL_H */
