/**
 * heap.h - kenrel heap
 * System sources under license MIT
 */

#ifndef HEAP_H_
# define HEAP_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief return the physical page size
 * @details it is accessible via the global object
 * but this is not guarantee to be the case in the future.
 * Prefer to use this getter instead.
 */
size_t	get_page_size();

void	*get_new_heap_page();

void	*kmalloc(size_t);
void	*kmalloc_physical(size_t, uint32_t*);

void	kfree(void *);

#endif // HEAP_H_

