A simple Multiboot-compliant "hello world kernel" with ring 0/ring 3 
multitasking and loadable ELF-modules.

# How to compile

Edit the Makefile and change "TOOLCHAIN_PREFIX" if your gcc is not called 
i586-elf-gcc. Then run "make" to compile.

Requires libgcc and some standard headers (stdarg.h, stddef.h, stdint.h) 
that should have come with your compiler.

# How to test

Load build/i386/incos-i386-kernel with GRUB.
