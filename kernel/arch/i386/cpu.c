#include "arch/cpu.h"

unsigned int cpu_features;
struct cpuid_result
{
	unsigned int eax, ebx, ecx, edx;
};

struct cpuid_result cpuid(unsigned int eax)
{
	struct cpuid_result result;
	__asm__("cpuid" : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx), "=d"(result.edx)
		: "a"(eax));
	return result;
}

unsigned int read_eflags()
{
	unsigned int result;
	__asm__("pushf\n\t"
			"pop %0\n\t"
			: "=r"(result));
	return result;
}

void cpu_init()
{
	cpu_features = 0;

	// if (!(read_eflags() & EFLAGS_ID)) return;

	//printf("cpuid(%x) = { %08x, %08x, %08x, %08x }\n", 0x00000000, cpuid(0x00000000).eax, cpuid(0x00000000).ebx, cpuid(0x00000000).ecx, cpuid(0x00000000).edx);
	//printf("cpuid(%x) = { %08x, %08x, %08x, %08x }\n", 0x80000001, cpuid(0x80000001).eax, cpuid(0x80000001).ebx, cpuid(0x80000001).ecx, cpuid(0x80000001).edx);
	//if (cpuid(0x80000000).eax < 0x80000001)
	//	return;

	//cpu_features = cpuid(0x80000001).edx;
	cpu_features = CPU_PSE;
}
