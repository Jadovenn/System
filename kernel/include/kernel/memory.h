/**
 * memory.h - Memory management
 * System sources under license MIT
 */

#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <stddef.h>

/**
 * Alloc memory from the heap
 * @param bytes - Quantity of byte to allocate
 * @return
 */
void* malloc(size_t bytes);

/**
 * Free a memory heap block
 * @param block - block ptr to free
 */
void free(void* block);

/**
 * Alloc memory from the kernel data area
 * @details Stands for long term and big allocation
 * @attention Physical pages are claim only while memory is accessed
 * @param bytes - Quantity of byte to allocate
 * @return
 */
void* vmalloc(size_t bytes);

/**
 * Free a data area memory block
 * @param block - block ptr to free
 */
void vfree(void* block);

#endif // KERNEL_MEMORY_H
