#include "elf.h"
#include "kernel.h"
#include "process.h"

void
run_elf_image(paddr_t base)
{
	process_t *process;
	struct elf_header *elf_hdr;
	struct elf_prog_header *prog_hdr;
	
	process = create_process();

	elf_hdr = (struct elf_header *)base;

	for (int i = 0; i < elf_hdr->e_phnum; i++) {
		prog_hdr = (struct elf_prog_header *)(base + elf_hdr->e_phoff + i * elf_hdr->e_phentsize);
		
		if (prog_hdr->p_type == PT_LOAD) {
			if (prog_hdr->p_offset & PAGE_MASK) {
				kprintf("run_boot_module: section not aligned\n");
				return;
			}

			if (PAGE_ROUND(prog_hdr->p_filesz) != 0) {
				kprintf("load section\t%08x -> %08x [%08x Bytes]\n", base + prog_hdr->p_offset, prog_hdr->p_vaddr, PAGE_ROUND(prog_hdr->p_filesz));
				process_map(process, prog_hdr->p_vaddr, base + prog_hdr->p_offset, PAGE_ROUND(prog_hdr->p_filesz));

				if (prog_hdr->p_filesz & PAGE_MASK) {
					kprintf("run_boot_module: prog_hdr->p_filesz & PAGE_MASK\n");
					//vm_fpt_zero(p_vaddr + p_filesz, PAGE_ROUND(p_filesz) - p_filesz)
				}
			}

			if (PAGE_ROUND(prog_hdr->p_memsz) - PAGE_ROUND(prog_hdr->p_filesz) != 0) {
				kprintf("zero section\t         -> %08x [%08x Bytes]\n", prog_hdr->p_vaddr + PAGE_ROUND(prog_hdr->p_filesz), PAGE_ROUND(prog_hdr->p_memsz) - PAGE_ROUND(prog_hdr->p_filesz));
				process_zero_mem(process, prog_hdr->p_vaddr + PAGE_ROUND(prog_hdr->p_filesz), PAGE_ROUND(prog_hdr->p_memsz) - PAGE_ROUND(prog_hdr->p_filesz));
			}
		}
	}

	create_thread(process, (void*)elf_hdr->e_entry);
}
