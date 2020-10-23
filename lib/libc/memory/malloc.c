/**
 * malloc.c - Memory Allocator
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>

#include "heapBMmalloc.h"

static _heapBM_t heap;
void* (*get_block)(size_t*, size_t);
#define CHUNCK_SIZE 16

void libc_init_allocator(uintptr_t vstart, size_t size,
                         void* (*cb)(size_t*, size_t)) {
	_heapBM_init(&heap);
	_heapBM_add_block(&heap, vstart, size, CHUNCK_SIZE);
	get_block = cb;
}

void* malloc(size_t size) {
	void* ptr = _heapBM_alloc(&heap, size);
	if (!ptr && get_block) {
		size_t new_blk_size;
		size_t request =
				size + sizeof(_heapBM_block_t) +
				(size % CHUNCK_SIZE ? size / CHUNCK_SIZE + 1 : size / CHUNCK_SIZE);
		ptr = get_block(&new_blk_size, request);
		if (!ptr || !new_blk_size) {
			return NULL;
		}
		_heapBM_add_block(&heap, (uintptr_t)ptr, new_blk_size, CHUNCK_SIZE);
		return _heapBM_alloc(&heap, size);
	}
	return ptr;
}

void free(void* ptr) { _heapBM_free(&heap, ptr); }
