#ifndef KERNEL_ARCH_X86_IDT_H
#define KERNEL_ARCH_X86_IDT_H

#include "types.h"

typedef struct idtr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtr_t;


typedef struct idt_descriptor {
	uint16_t offset;
	uint16_t selector;
	uint16_t flags;
	uint16_t offset16;
} __attribute__((packed)) idt_descriptor_t;

#define IDT_INTERRUPT_GATE  0x8E00
#define IDT_TRAP_GATE       0x8F00
#define IDT_TASK_GATE       0x8500

#define IDT_SYSCALL         0xEE00

struct stack_frame
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, _esp, ebx, edx, ecx, eax;
    unsigned int interrupt, error;
    unsigned int eip, cs, eflags, esp, ss;
};


#endif /* ndef KERNEL_ARCH_X86_IDT_H */
