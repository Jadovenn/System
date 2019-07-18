/**
 * heap.c - kenrel heap
 * System sources under license MIT
 */

#ifndef HEAP_H_
# define HEAP_H_


#include <kernel/types.h>

void	*kmalloc(size_t);

void	kfree(void *);

#endif // HEAP_H_

