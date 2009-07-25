#include "syscall.h"

extern int dprintf(const char *, ...);

int main(int argc, char **argv)
{
	dprintf("thread %d process %d hello from %08x\n", syscall_gettid(), syscall_getpid(), main);
	dprintf("argc = %d\n", argc);
	for (int i = 0; i < argc; i++)
		dprintf("argv[%d] = %s\n", i, argv[i]);
	
	return 123;
}
