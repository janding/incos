#ifndef ARCH_CPU_H
#define ARCH_CPU_H

#include <types.h>

/*#define EFLAGS_ID		0x00200000

#define CPUID_PSE36		0x00020000
#define CPUID_PAT		0x00010000
#define CPUID_SEP		0x00000800
#define CPUID_PAE		0x00000040
#define CPUID_MSR		0x00000020
#define CPUID_TSC		0x00000010
#define CPUID_PSE		0x00000008*/

#define CPU_PSE			0x00000008

extern unsigned int cpu_features;

__attribute__((always_inline))
static inline void 
write_cr0(uint32_t value)  
{
	__asm__("mov %0, %%cr0\n\t" :: "r"(value));
}

__attribute__((always_inline))
static inline uint32_t
read_cr0(void) 
{
	unsigned int result;
	__asm__("mov %%cr0, %0\n\t" : "=r"(result));
	return result;
}


static inline uint32_t
read_cr2(void)
{
	unsigned int result;
	__asm__("mov %%cr2, %0\n\t" : "=r"(result));
	return result;
}

__attribute__((always_inline))
static inline void 
write_cr3(uint32_t value)  
{
	__asm__("mov %0, %%cr3\n\t" :: "r"(value));
}

static inline uint32_t
read_cr3(void)
{
	unsigned int result;
	__asm__("mov %%cr3, %0\n\t" : "=r"(result));
	return result;
}


static inline void 
write_cr4(uint32_t value)
{
	__asm__("mov %0, %%cr4\n\t" :: "r"(value));
}

static inline uint32_t
read_cr4(void)
{
	unsigned int result;
	__asm__("mov %%cr4, %0\n\t" : "=r"(result));
	return result;
}


#endif /* ndef ARCH_CPU_H */
