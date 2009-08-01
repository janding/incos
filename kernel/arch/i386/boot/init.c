#include <types.h>

#include "kernel.h"
#include "multiboot.h"
	
extern void cons_init();
extern void gdt_init();
extern void idt_init();
extern void kern_init();
extern void pic_init();
extern void tss_init();
extern void rtc_init();

extern void run_elf_image(paddr_t base, const char *cmdline);

void
run_boot_modules()
{
	multiboot_module_t * modules = (multiboot_module_t *)multiboot_info->mods_addr;

	for (int i = 0; i < multiboot_info->mods_count; i++) {
		run_elf_image(modules[i].mod_start, (const char *)modules[i].string);
	}
}

__global
void 
init(uint32_t mb_magic, multiboot_info_t *mb_info)
{
	cons_init();

	if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		kprintf("multiboot error");
		return;
	}

	multiboot_info = mb_info;

	gdt_init();
	idt_init();
	tss_init();
	pic_init();
	rtc_init();

	kern_init();
	for (;;);
}
