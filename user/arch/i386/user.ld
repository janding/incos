OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
INPUT(build/i386/crt0.o)
GROUP(build/i386/libuser.a)
SECTIONS
{
	. = 0x400000;
	.text : ALIGN(4096) { *(.text) *(.rdata) *(.rodata) }
	.data : ALIGN(4096) { *(.data) }
	.bss : ALIGN(4096) { *(.bss) }
}
