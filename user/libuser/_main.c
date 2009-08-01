#include "syscall.h"

extern int main(int argc, char **argv);

void exit(int i)
{
	syscall_exit(i);
}

void _main(char *cmdline)
{
	int argc = 0;
	char *argv[2];

	argc = 1;
	argv[0] = cmdline;
	argv[1] = 0;

	exit(main(argc, argv));
}
