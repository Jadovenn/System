/**
 * va_arg.h - x86 variadic function arguments
 * System sources under license MIT
 */

#ifndef VA_ARG_H_
# define VA_ARG_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __i386__

static inline unsigned	__align_sizeof(uint8_t size) {
	int	aligned_value = 8;
	if (size <= 4) {
		return 4;
	}
	while (size > aligned_value)
		aligned_value = aligned_value << 1;
	return aligned_value;
}

static inline void	*__read_arg(char **address, uint8_t size) {
	char *current_arg = *address;
	*address = *address + __align_sizeof(size);
	return current_arg;
}

typedef char *va_list;

#define va_start(list, first_arg) (list = ((char*)&first_arg + __align_sizeof(sizeof(first_arg))))

//#define va_arg(list, mode) ((mode *)(list = (char *)list + __align_sizeof(sizeof(mode))))[-1]

#define va_arg(list, mode) ((mode*)__read_arg(&list, sizeof(mode)))[0]

#define va_end(list) (list = NULL)

#endif // __i386__

#endif // VA_ARG_H_
