A simple Multiboot-compliant *hello world kernel* with ring 0/ring 3 
multitasking and loadable ELF-modules.

# Requirements

Requires a 32-bit [cross-compiler](https://wiki.osdev.org/GCC_Cross-Compiler)
with libgcc and some standard headers (stdarg.h, stddef.h, stdint.h) 
that should have come with your compiler.

# How to compile

Edit the Makefile and change `TOOLCHAIN_PREFIX` if your gcc is not called 
`i586-elf-gcc`. Then run `make` to compile.

# How to test

Load build/i386/incos-i386-kernel with GRUB or QEMU.

The following command loads the kernel and two instances of the *hello*
program.

```
qemu-system-x86_64 -kernel build/i386/incos-i386-kernel -initrd build/i386/hello,build/i386/hello
```
