/**
 * heap.h - kernel memory allocation API
 * System sources under license MIT
 */

#ifndef HEAP_H_
# define HEAP_H_

// vanilla malloc
uint32_t	kmalloc(uint32_t size);

// page aligned
uint32_t	kmalloc_aligned(uint32_t size);

// return physical address
uint32_t	kmalloc_physical(uint32_t size, uint32_t *phys);

// page aligned and return physical address
uint32_t	kmalloc_physical_align(uint32_t size, uint32_t *phys);

#endif // HEAP_H_

