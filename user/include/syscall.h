#ifndef SYSCALL_H
#define SYSCALL_H

#include "syscalls.h"

#define  __fastcall __attribute__((__fastcall__)

#define syscall0(number) ({ \
		int result; \
		__asm__ __volatile__("int $0x30\n\t" : "=a"(result) : "a"(number) : "ebx", "ecx", "edx"); \
		result; \
	})

#define syscall1(number, param1) ({ \
		int result, temp; \
		__asm__ __volatile__("int $0x30\n\t" : "=a"(result), "=c"(temp) : "a"(number), "c"(param1) : "ebx", "edx"); \
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

static inline void
syscall_sleep(int seconds)
{
	syscall1(SYSCALL_SLEEP, seconds);
}

static inline unsigned long long
syscall_rdtsc()
{
	unsigned long long result; 
	__asm__ __volatile__("int $0x31\n\t" : "=A"(result));
	return result;
}

#endif /* ndef SYSCALL_H */
