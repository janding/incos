#ifndef SYSCALL_H
#define SYSCALL_H

#include "syscalls.h"

extern int syscall(unsigned int number);

int		syscall_dprint(const char *s);
int		syscall_gettid(void);
int		syscall_getpid(void);

#endif /* ndef */
