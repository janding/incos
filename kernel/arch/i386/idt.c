#include "arch/cpu.h"
#include "arch/irq.h"
#include "arch/idt.h"
#include "arch/pic.h"
#include "kernel.h"

void do_syscall(uint32_t *esp, struct stack_frame *sf);


idt_descriptor_t idt[256];
idtr_t idtr = { sizeof(idt) - 1, (uint32_t)idt };

void idt_set_entry(int i, uint32_t offset, uint32_t selector, uint32_t flags)
{
    idt[i].offset = offset & 0xFFFF;
    idt[i].selector = selector;
    idt[i].flags = flags;
    idt[i].offset16 = (offset >> 16) & 0xFFFF;
}

void set_intr_gate(int i, uint32_t offset)
{
    idt_set_entry(i, offset, 0x08, IDT_INTERRUPT_GATE);
}

void set_intr_syscall(int i, uint32_t offset)
{
    idt_set_entry(i, offset, 0x08, IDT_SYSCALL);
}

extern void  interrupt_stub_0();
extern void  interrupt_stub_1();
extern void  interrupt_stub_2();
extern void  interrupt_stub_3();
extern void  interrupt_stub_4();
extern void  interrupt_stub_5();
extern void  interrupt_stub_6();
extern void  interrupt_stub_7();
extern void  interrupt_stub_8();
extern void  interrupt_stub_9();
extern void interrupt_stub_10();
extern void interrupt_stub_11();
extern void interrupt_stub_12();
extern void interrupt_stub_13();
extern void interrupt_stub_14();
extern void interrupt_stub_16();
extern void interrupt_stub_17();
extern void interrupt_stub_18();
extern void interrupt_stub_19();
extern void interrupt_stub_32();
extern void interrupt_stub_33();
extern void interrupt_stub_34();
extern void interrupt_stub_35();
extern void interrupt_stub_36();
extern void interrupt_stub_37();
extern void interrupt_stub_38();
extern void interrupt_stub_39();
extern void interrupt_stub_40();
extern void interrupt_stub_41();
extern void interrupt_stub_42();
extern void interrupt_stub_43();
extern void interrupt_stub_44();
extern void interrupt_stub_45();
extern void interrupt_stub_46();
extern void interrupt_stub_47();
extern void interrupt_stub_48();

void idt_init()
{
	set_intr_gate(0, (uint32_t)&interrupt_stub_0);
	set_intr_gate(1, (uint32_t)&interrupt_stub_1);
	set_intr_gate(2, (uint32_t)&interrupt_stub_2);
	set_intr_gate(3, (uint32_t)&interrupt_stub_3);
	set_intr_gate(4, (uint32_t)&interrupt_stub_4);
	set_intr_gate(5, (uint32_t)&interrupt_stub_5);
	set_intr_gate(6, (uint32_t)&interrupt_stub_6);
	set_intr_gate(7, (uint32_t)&interrupt_stub_7);
	set_intr_gate(8, (uint32_t)&interrupt_stub_8);
	set_intr_gate(9, (uint32_t)&interrupt_stub_9);
	set_intr_gate(10, (uint32_t)&interrupt_stub_10);
	set_intr_gate(11, (uint32_t)&interrupt_stub_11);
	set_intr_gate(12, (uint32_t)&interrupt_stub_12);
	set_intr_gate(13, (uint32_t)&interrupt_stub_13);
	set_intr_gate(14, (uint32_t)&interrupt_stub_14);
	set_intr_gate(16, (uint32_t)&interrupt_stub_16);
	set_intr_gate(17, (uint32_t)&interrupt_stub_17);
	set_intr_gate(18, (uint32_t)&interrupt_stub_18);
	set_intr_gate(19, (uint32_t)&interrupt_stub_19);

	set_intr_gate(32, (uint32_t)&interrupt_stub_32);
	set_intr_gate(33, (uint32_t)&interrupt_stub_33);
	set_intr_gate(34, (uint32_t)&interrupt_stub_34);
	set_intr_gate(35, (uint32_t)&interrupt_stub_35);
	set_intr_gate(36, (uint32_t)&interrupt_stub_36);
	set_intr_gate(37, (uint32_t)&interrupt_stub_37);
	set_intr_gate(38, (uint32_t)&interrupt_stub_38);
	set_intr_gate(39, (uint32_t)&interrupt_stub_39);
	set_intr_gate(40, (uint32_t)&interrupt_stub_40);
	set_intr_gate(41, (uint32_t)&interrupt_stub_41);
	set_intr_gate(42, (uint32_t)&interrupt_stub_42);
	set_intr_gate(43, (uint32_t)&interrupt_stub_43);
	set_intr_gate(44, (uint32_t)&interrupt_stub_44);
	set_intr_gate(45, (uint32_t)&interrupt_stub_45);
	set_intr_gate(46, (uint32_t)&interrupt_stub_46);
	set_intr_gate(47, (uint32_t)&interrupt_stub_47);
	
	set_intr_syscall(48, (uint32_t)&interrupt_stub_48);

	__asm__("lidt %[idtr]\n\t" :: [idtr] "m"(idtr));
}

const char * exceptions[] = { "DE", "DB", "NMI", "BP", "OF", "BR", "UD", "NM", "DF", "CO", "TS", "NP", "SS", "GP", "PF", "(reserved)", "MF", "AC", "MC", "XM" };

uint32_t int_handler(uint32_t _esp)
{
	uint32_t esp = _esp;
	struct stack_frame *sf = (struct stack_frame*)esp;

	if (sf->interrupt < 0x20) {
		kprintf("EXCEPTION");		
		if (sf->interrupt < sizeof(exceptions) / sizeof(*exceptions))
			kprintf(" (#%s)", exceptions[sf->interrupt]);
		kprintf("\n");

		kprintf("eax = %08x\tebx = %08x\tecx = %08x\tedx = %08x\n", sf->eax, sf->ebx, sf->ecx, sf->edx);
		kprintf("edi = %08x\tesi = %08x\tebp = %08x\t_esp = %08x\n", sf->edi, sf->esi, sf->ebp, sf->_esp);
		kprintf("eip = %08x\terr = %08x\tintr = %08x\teflags = %08x\n", sf->eip, sf->error, sf->interrupt, sf->eflags);
		if (sf->cs & 3)
			kprintf("ss = %04x  esp = %08x\n", sf->ss, sf->esp);
		kprintf("cs = %04x  ds = %04x  es = %04x  fs = %04x  gs = %04x\n", sf->cs, sf->ds, sf->es, sf->fs, sf->gs);
		kprintf("cr2 = %08x\n", read_cr2());
		for(;;);
	}

	if (IRQ_BASE <= sf->interrupt && sf->interrupt <= IRQ_BASE + 15) {
		handle_irq(&esp, sf->interrupt - IRQ_BASE);
	}

	if (sf->interrupt == 0x30) {
		do_syscall(&esp, sf);
	}

	return esp;
}
