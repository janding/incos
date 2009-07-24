#ifndef KERNEL_ARCH_X86_PIO_H
#define KERNEL_ARCH_X86_PIO_H

static inline void io_wait()
{
	__asm__ __volatile__("jmp 1f\n1:\tjmp 1f\n1:");
}

static inline void outb(unsigned short port, unsigned char val)
{
	__asm__ __volatile__("outb %0, %1" : : "a" (val), "Nd" (port));
}

static inline void outb_wait(unsigned short port, unsigned char val)
{
	outb(port, val);
	io_wait();
}

static inline unsigned char inb(unsigned short port)
{
	unsigned char val;
	__asm__ __volatile__("inb %1, %0" : "=a" (val) : "Nd" (port));
	return val;
}

static inline unsigned char inb_wait(unsigned short port)
{
	unsigned char val = inb(port);
	io_wait();
	return val;
}


#endif /* ndef KERNEL_ARCH_X86_PIO_H */
