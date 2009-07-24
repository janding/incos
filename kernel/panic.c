#include <stdarg.h>

#include "kernel.h"

extern int vkprintf(const char *format, va_list ap);

void panic(const char *message, ...)
{
	va_list ap;

	kprintf("panic: ");

	va_start(ap, message);
	vkprintf(message, ap);
	va_end(ap);

	for (;;);
}
