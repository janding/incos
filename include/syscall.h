#ifndef SYSCALL_H
#define SYSCALL_H

#include "kernel.h"
#include "syscalls.h"

//extern int syscall(unsigned int number);

extern __fastcall void		syscall_exit(int e);
extern __fastcall int		syscall_dprint(const char *s);
extern __fastcall int		syscall_gettid(void);
extern __fastcall int		syscall_getpid(void);
extern __fastcall void		syscall_sleep(int seconds);

#endif /* ndef */
