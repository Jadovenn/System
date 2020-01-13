/**
 * physical_memory.c - Physical Memory Management
 * System sources under license MIT
 */

#include <stddef.h>

extern const char *_end;

struct physical_memory_group *_kmem;

void	pmm_init1(void *vstart, void *vend) {
	(void)vstart;
	(void)vend;
	_kmem = NULL;
}

void	physical_free(void) {

}

void	*physical_alloc(void) {
	return NULL;
}

