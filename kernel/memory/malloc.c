/**
 * malloc.h - General purpose allocator
 * System sources under license MIT
 */

#include <stdint.h>
#include <string.h>

#include <hal/memory.h>
#include <kernel/memory.h>
#include <kernel/panic.h>

#define CHUNK_SIZE 16

typedef struct Heap_block {
	struct Heap_block* next;
	uint32_t           size;
	uint32_t           used;
	uint32_t           bsize;
	uint32_t           lfb;
} Heap_block_t;

typedef struct Heap {
	Heap_block_t* block;
} Heap_t;

Heap_t heap;

void Heap_add_block(Heap_t*   heap,
                    uintptr_t addr,
                    uint32_t  size,
                    uint32_t  block_size) {
	Heap_block_t* block;
	uint32_t      block_count = 0;
	uint8_t*      bm          = NULL;

	block        = (Heap_block_t*)addr;
	block->size  = size - sizeof(Heap_block_t);
	block->bsize = block_size;
	block->next  = heap->block;
	heap->block  = block;
	block_count  = block->size / block->bsize;
	bm           = (uint8_t*)&block[1];
	memset(bm, block_count, 0);
	block_count = (block_count / block_size) * block_size < block_count
										? block_count / block_size + 1
										: block_count / block_size;
	for (unsigned idx = 0; idx < block_count; ++idx) {
		bm[idx] = 5;
	}
	block->lfb  = block_count - 1;
	block->used = block_count;
}

static __inline__ uint8_t Heap_get_node_id(uint8_t a, uint8_t b) {
	uint8_t c;
	for (c = a + 1; c == b || c == 0; ++c)
		;
	return c;
}

void* Heap_alloc(Heap_t* heap, size_t size) {
	Heap_block_t* block;
	uint8_t*      bm;
	uint32_t      blk_count;
	uint32_t      blk_need;
	uint8_t       nid;

	for (block = heap->block; block; block = block->next) {
		if (block->size - (block->used * block->bsize) >=
		    size) { // Regular case: block as enough slot ?
			blk_count = block->size / block->bsize;
			// align request to this block padding
			blk_need = (size / block->bsize) * block->bsize < size
										 ? size / block->bsize + 1
										 : size / block->bsize;
			bm       = (uint8_t*)&block[1];
			for (unsigned x = (block->lfb + 1 >= blk_count ? 0 : block->lfb + 1);
			     x < block->lfb; ++x) {
				if (x >= blk_count) {
					x = 0;
				}
				if (bm[x] == 0) {
					uint32_t free_blk = 0;
					for (; bm[x + free_blk] == 0 && free_blk < blk_need &&
					       (x + free_blk) < blk_count;
					     ++free_blk)
						;
					if (free_blk == blk_need) { // Regular case: free block(s) founded !
						nid = Heap_get_node_id(bm[x - 1], bm[x + free_blk]);
						for (unsigned idx = 0; idx < free_blk; ++idx) {
							bm[x + idx] = nid;
						}
						block->lfb = (x + blk_need) - 2;
						block->used += free_blk;
						return (void*)(x + block->size + (uintptr_t)&block[1]);
					}
				}
			}
		}
	}
	// Special case: not enough memory
	return NULL;
}

void Heap_free(Heap_t* heap, void* ptr) {
	Heap_block_t* block;
	uintptr_t     ptr_offset;
	uint32_t      bi, x;
	uint8_t*      bm;
	uint8_t       id;
	uint32_t      max;

	for (block = heap->block; block; block = block->next) {
		if ((uintptr_t)ptr > (uintptr_t)block &&
		    (uintptr_t)ptr <
		        (uintptr_t)block + sizeof(Heap_block_t) + block->size) {
			ptr_offset = (uintptr_t)((uint32_t)ptr - (uint32_t)&block[1]);
			bi         = (uint32_t)ptr_offset / block->size;
			bm         = (uint8_t*)&block[1];
			id         = bm[bi];
			max        = block->size / block->bsize;
			for (x = bi; bm[x] == id && x < max; ++x) {
				bm[x] = 0;
			}
			block->used -= x - bi;
			return;
		}
	}
}

void Hal_init_memory_allocator(uintptr_t vstart, size_t size) {
	Heap_add_block(&heap, vstart, size, CHUNK_SIZE);
}

void* malloc(size_t bytes) {
	if (!heap.block) {
		PANIC("Memory allocator is not initialized");
	}
	void* ptr = Heap_alloc(&heap, bytes);
	if (!ptr) {
		size_t request =
				bytes + sizeof(Heap_block_t) +
				(bytes % CHUNK_SIZE ? bytes / CHUNK_SIZE + 1 : bytes / CHUNK_SIZE);
		// ptr = get_block(&new_blk_size, request);
		if (!ptr) {
			return NULL;
		}
		Heap_add_block(&heap, (uintptr_t)ptr, request, CHUNK_SIZE);
		return Heap_alloc(&heap, bytes);
	}
	return ptr;
}

void free(void* block) {
	if (!heap.block) {
		PANIC("Memory allocator is not initialized");
	}
	Heap_free(&heap, block);
}
