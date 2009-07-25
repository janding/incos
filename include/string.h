#ifndef STRING_H
#define STRING_H

#include "types.h"

static inline size_t 
strlen(const char *str)
{
	size_t i = 0;
	while (str[i])
		i++;
	return i;
}

static inline size_t
strlcpy(char *destination, const char *source, size_t num)
{
	size_t len = 0;
	
	if (num == 0)
		return strlen(source);

	do {
		*destination++ = *source++;
		len++;
	} while (len < num && *source);
	
	if (len == num)
		*destination = 0;

	while (*source++)
		len++;

	return len;
}


#endif /* ndef STRING_H */
