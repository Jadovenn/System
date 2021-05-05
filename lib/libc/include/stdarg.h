/**
 * stdarg.h - handle variable argument list
 * System sources under license MIT
 */

#ifndef STDARG_H_
#define STDARG_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __i386__

static inline unsigned __align_sizeof(uint8_t size) {
	int aligned_value = 8;
	if (size <= 4) {
		return 4;
	}
	while (size > aligned_value)
		aligned_value = aligned_value << 1;
	return aligned_value;
}

static inline void* __read_arg(char** address, uint8_t size) {
	char* current_arg = *address;
	*address = *address + __align_sizeof(size);
	return current_arg;
}

/**
 * @brief type for variables used to traverse the list
 */
typedef char* va_list;

/**
 * @brief initialize ap to the beginning of the list before any calls to
 * va_arg()
 */
#define va_start(list, first_arg)                                              \
	(list = ((char*)&first_arg + __align_sizeof(sizeof(first_arg))))

/**
 * @brief initializes dest as a copy of src, as if the va_start()
 * 	macro had been applied to dest followed by the same sequence of uses of the
 * va_arg()
 */
#define va_copy(dest, copy) (dest = copy)

/**
 * @brief shall return the next argument in the list pointed to by ap
 */
#define va_arg(list, mode) ((mode*)__read_arg(&list, sizeof(mode)))[0]

/**
 * @brief clean up, don't do anything after that call
 */
#define va_end(list) (list = NULL)

#endif // __i386__

#endif // STDARG_H_
