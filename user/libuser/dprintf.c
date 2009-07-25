#include <stdarg.h>
#include <stddef.h>

#include "syscall.h"

extern int cons_puts(const char*);

static int 
abs(int i) 
{ 
	return i < 0 ? -i : i; 
}

static int
putstr(const char* s)
{
	return syscall_dprint(s);
}

static int 
putchar(int c)
{
	char b[2] = { c, 0 };
	syscall_dprint(b);
	return c;
}

const char * 
lltoa(long long n, char* buffer, int radix, char pad, int width, int uppercase)
{
	const char *digits = uppercase ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
								   : "0123456789abcdefghijklmnopqrstuvwxyz";
	char tbuf[130], *p = tbuf, *d;
	int neg = 0;

	if ((n < 0) && (radix == 10)) {
		neg = -1;
		width--;
	}

	do {
		*p++ = digits[abs(n % radix)];
		width--;
	} while (n /= radix);

	if (pad != ' ') while (width-- > 0) *p++ = pad;

	if (neg) *p++ = '-';

	if (pad == ' ') while (width-- > 0) *p++ = pad;

	// reverse buffer
	d = buffer;
	while (p > tbuf) {
		*d++ = *--p;
	}
	*d = 0;

	return buffer;
}

const char * 
ulltoa(unsigned long long n, char* buffer, int radix, char pad, int width, int uppercase)
{
	const char *digits = uppercase ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
								   : "0123456789abcdefghijklmnopqrstuvwxyz";
	char tbuf[130], *p = tbuf, *d;

	do {
		*p++ = digits[n % radix];
		width--;
	} while (n /= radix);

	while (width-- > 0) *p++ = pad;

	// reverse buffer
	d = buffer;
	while (p > tbuf) {
		*d++ = *--p;
	}
	*d = 0;

	return buffer;
}


int 
vdprintf(const char *format, va_list ap)
{
	int result = 0;
	char buffer[130];
	enum { SHORT, INT, LONG, LONGLONG } type;
	int width, sign, radix, uppercase;
	char pad;

	if (format == NULL)
		return 0;

	while (*format) {
		if (*format == '%') {
			pad = ' ';
			width = 0;
			type = INT;
			sign = 0;
			uppercase = 0;
			radix = 10;
			format++;

			if (*format == '0') {
				pad = '0';
				format++;
			}

			while ('0' <= *format && *format <= '9') {
				width = width * 10 + *format - '0';
				format++;
			}

			if (*format == 'l') {
				type = LONG;
				format++;
				if (*format == 'l') {
					type = LONGLONG;
					format++;
				}
			}


			switch (*format) {
				case '%':
					putchar('%');
					result++;
					break;
				case 'b': sign = 0; radix = 2; goto number;
				case 'o': sign = 0; radix = 8; goto number;
				case 'i':
				case 'd': sign = 1; radix = 10; goto number;
				case 'u': sign = 0; radix = 10; goto number;
				case 'x': sign = 0; radix = 16; uppercase = 0; goto number;
				case 'X': sign = 0; radix = 16; uppercase = 1; goto number;

number:
					if (sign) {
						switch (type) {
							case SHORT:		result += putstr(lltoa(va_arg(ap, int), buffer, radix, pad, width, uppercase)); break;
							case INT:		result += putstr(lltoa(va_arg(ap, int), buffer, radix, pad, width, uppercase)); break;
							case LONG:		result += putstr(lltoa(va_arg(ap, long), buffer, radix, pad, width, uppercase)); break;
							case LONGLONG:	result += putstr(lltoa(va_arg(ap, long long), buffer, radix, pad, width, uppercase)); break;
						}
					} else {
						switch (type) {
							case SHORT:		result += putstr(ulltoa(va_arg(ap, unsigned int), buffer, radix, pad, width, uppercase)); break;
							case INT:		result += putstr(ulltoa(va_arg(ap, unsigned int), buffer, radix, pad, width, uppercase)); break;
							case LONG:		result += putstr(ulltoa(va_arg(ap, unsigned long), buffer, radix, pad, width, uppercase)); break;
							case LONGLONG:	result += putstr(ulltoa(va_arg(ap, unsigned long long), buffer, radix, pad, width, uppercase)); break;
						}
					}
					break;
				case 's':
					result += putstr(va_arg(ap, char*));
					break;
				default:
					break;
			}

		} else {
			putchar(*format);
			result++;
		}
		format++;
	}

	return result;
}

int 
dprintf(const char *format, ...)
{
	va_list ap;
	int result;

	if (format == NULL)
		return 0;

	va_start(ap, format);
	result = vdprintf(format, ap);
	va_end(ap);

	return result;
}
