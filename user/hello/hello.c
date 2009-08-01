#include "syscall.h"

extern int dprintf(const char *, ...);

unsigned long long rdtsc()
{ 
	unsigned long long retval;
	__asm__ __volatile__("rdtsc" : "=A"(retval));
	return retval;
}

unsigned long long measure_syscall()
{
	unsigned long long start, end;
	start = rdtsc();
	end = syscall_rdtsc();
	return end - start;
}

int main(int argc, char **argv)
{
	while (1) {
		//dprintf("%d ", measure_syscall());
		dprintf("%d", syscall_gettid());
		syscall_sleep(1);
	}
	
	//return 123;
}
