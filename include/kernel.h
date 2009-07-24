#ifndef KERNEL_H
#define KERNEL_H

extern int	kprintf(const char*, ...);

__attribute__((__noreturn__))
extern void	panic(const char *, ...);

#ifndef NULL
#define NULL 0
#endif

#endif /* ndef KERNEL_H */
