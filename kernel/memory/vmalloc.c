/**
 * vmalloc.h - Virtual memory allocator
 * System sources under license MIT
 */

#include <kernel/memory.h>

void* vmalloc(size_t bytes) {
	(void)bytes;
	return NULL;
}

void vfree(void* block) {
	(void)block;
}
