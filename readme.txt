A simple Multiboot compliant Hello World Kernel with Ring 0/Ring 3 
Multitasking and loadable ELF-modules.

How to compile:

Edit the Makefile and change "TOOLCHAIN_PREFIX" if your gcc is not called 
i586-elf-gcc. Then run "make" to compile.

Required libgcc and some Standard Headers (stdarg.h, stddef.h, stdint.h).

How to test:

Load build/i386/incos-i386-kernel with GRUB.