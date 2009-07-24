OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
_VTOP = 0xf0000000;
SECTIONS
{
	. = 0x100000 + SIZEOF_HEADERS;

	kernel_phys_start = .;
	kernel_virt_start = kernel_phys_start + _VTOP;
	
	.boot :
	{
		*entry.o *multiboot.o *vm_boot.o(.text .data .bss)
		. = ALIGN(0x1000);
	}
	
	. = . + _VTOP;

	.text : AT(ADDR(.boot) + SIZEOF(.boot))
	{
		*(.text)
		*(.rdata)
		*(.rodata)
	}
	
	.data :
	{
		__data_start = .;
		*(.data)
	}

	.bss :
	{
		*(.data)
	}
	__end = .;

	kernel_virt_end = .;
	kernel_phys_end = kernel_virt_end - _VTOP;
	
	/DISCARD/ : { *(.comment) }
}
