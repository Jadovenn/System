/**
 * allocator.h - memory allocator interface
 * System sources under license MIT
 */

#ifndef SYSTEM_ALLOCATOR_H
#define SYSTEM_ALLOCATOR_H

#include <stddef.h>

void* System_malloc(void* allocator, size_t size);

#endif // SYSTEM_ALLOCATOR_H
