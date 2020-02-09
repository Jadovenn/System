/**
 * unistd.h - standard symbolic constants and types
 * System sources under license MIT
 */


#ifndef _UNISTD_H_
# define _UINSTD_H_

// not a complete implmentation
#define _POSIX_VERSION -1
#define _POSIX2_VERSION -1

#include <stddef.h>
#include <stdint.h>

void	libc_init_allocator(uintptr_t vstart, size_t size, void*(*cb)(size_t*, size_t));
void	*malloc(size_t size);
void	free(void *ptr);

#endif // _UNISTD_H_

