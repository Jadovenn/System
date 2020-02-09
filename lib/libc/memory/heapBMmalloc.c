/**
 * heapBMmalloc.c - Bitmap heap Algorithm
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "heapBMmalloc.h"

void	_heapBM_init(_heapBM_t *heap) {
	heap->fblock = NULL;
}

void _heapBM_add_block(_heapBM_t *heap, uintptr_t addr, uint32_t size, uint32_t block_size) {
	_heapBM_block_t	*block;
	uint32_t	block_count = 0;
	uint8_t		*bm = NULL;

	block = (_heapBM_block_t*)addr;
	block->size = size - sizeof(_heapBM_block_t);
	block->bsize = block_size;
	block->next = heap->fblock;
	heap->fblock = block;
	block_count = block->size / block->bsize;
	bm = (uint8_t*)&block[1];
	memset(bm, block_count, 0);
	block_count = (block_count / block_size) * block_size < block_count ? block_count / block_size + 1 : block_count / block_size;
	for(unsigned idx = 0; idx < block_count; ++idx) {
		bm[idx] = 5;
	}
	block->lfb = block_count - 1;
	block->used = block_count;
}

static __inline__
uint8_t	_heapBM_get_node_id(uint8_t a, uint8_t b) {
	uint8_t	c;
	for (c = a + 1; c == b || c == 0; ++c);
	return c;
}

void	*_heapBM_alloc(_heapBM_t *heap, size_t size) {
	_heapBM_block_t	*block;
	uint8_t		*bm;
	uint32_t	blk_count;
	uint32_t	blk_need;
	uint8_t		nid;

	for (block = heap->fblock; block; block = block->next) {
		if (block->size - (block->used * block->bsize) >= size) { // Regular case: block as enought slot ?
			blk_count = block->size / block->bsize;
			// alligne request to this block padding
			blk_need = (size / block->bsize) * block->bsize < size ? size / block->bsize + 1 : size / block->bsize;
			bm = (uint8_t*)&block[1];
			for (unsigned x = (block->lfb + 1 >= blk_count ? 0 : block->lfb + 1); x < block->lfb ; ++x) {
				if (x >= blk_count) {
					x = 0;
				}
				if (bm[x] == 0) {
					uint32_t free_blk = 0;
					for (; bm[x + free_blk] == 0 && free_blk < blk_need && (x + free_blk) < blk_count; ++free_blk);
					if (free_blk == blk_need) { // Regular case: free block(s) founded !
						nid = _heapBM_get_node_id(bm[x - 1], bm[x + free_blk]);
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
	// Special case: not enought memory
	return NULL;
}

void	_heapBM_free(_heapBM_t *heap, void *ptr) {
	_heapBM_block_t	*block;
	uintptr_t	ptr_offset;
	uint32_t	bi, x;
	uint8_t		*bm;
	uint8_t		id;
	uint32_t	max;

	for (block = heap->fblock; block; block = block->next) {
		if ((uintptr_t)ptr > (uintptr_t)block && (uintptr_t)ptr < (uintptr_t)block + sizeof(_heapBM_block_t) + block->size) {
			ptr_offset = (uintptr_t)((uint32_t)ptr - (uint32_t)&block[1]);
			bi = (uint32_t)ptr_offset / block->size;
			bm = (uint8_t*)&block[1];
			id = bm[bi];
			max = block->size / block->bsize;
			for (x = bi; bm[x] == id && x < max; ++x) {
				bm[x] = 0;
			}
			block->used -= x - bi;
			return;
		}
	}
}

