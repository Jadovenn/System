/**
 * physical_memory.c - Physical Memory Management
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>

extern const char *_end;

struct physical_memory_group *_kmem;

void	pmm_init1(void *vstart, void *vend) {
	(void)vstart;
	(void)vend;
	_kmem = NULL;
}

void	physical_free(void) {

}

/**
 * @brief physical memory page allocator
 * @details sequencial search algorithm using shortcut ptr
 * @return a physical pointer to a 4Kib physical page
 */
void	*physical_alloc(void) {
	return NULL;
}

