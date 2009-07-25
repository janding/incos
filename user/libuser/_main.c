#include "syscall.h"
#include "string.h"

extern int main(int argc, char **argv);

void exit(int i)
{
	syscall_exit(i);
}

void _main(const char *cmdline)
{
	int argc;
	char *argv[2];

	argc = 1;
	argv[0] = cmdline;
	argv[1] = cmdline;

	exit(main(argc, argv));
}
