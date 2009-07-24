#ifndef ARCH_INTR_H
#define ARCH_INTR_H

#define enable_interrupts() __asm__ __volatile__ ("sti")
#define disable_interrupts() __asm__ __volatile__ ("cli")

#endif /* ndef ARCH_INTR_H */
