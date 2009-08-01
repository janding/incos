#ifndef KERNEL_H
#define KERNEL_H

extern int	kprintf(const char*, ...);

__attribute__((__noreturn__))
extern void	panic(const char *, ...);

#define __fastcall __attribute__((__fastcall__))
#define __global __attribute__((__externally_visible__))

#ifndef NULL
#define NULL 0
#endif

#endif /* ndef KERNEL_H */
