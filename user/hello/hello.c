#include "syscall.h"

extern int dprintf(const char *, ...);

int main()
{
	dprintf("thread %d process %d hello from %08x\n", syscall_gettid(), syscall_getpid(), main);
	for(;;);
}
