/**
 * heapBMmalloc.h - Bitmap heap Algorithm
 * System sources under license MIT
 */

#ifndef _HEAPBMMALLOC_H_
# define _HEAPBMMALLOC_H_

typedef struct	_heapBM_block {
	struct _heapBM_block	*next;
	uint32_t		size;
	uint32_t		used;
	uint32_t		bsize;
	uint32_t		lfb;
}		_heapBM_block_t;

typedef struct		_heapBM {
	_heapBM_block_t	*fblock;
}			_heapBM_t;

void	_heapBM_init(_heapBM_t *heap);
void 	_heapBM_add_block(_heapBM_t *heap, uintptr_t addr, uint32_t size, uint32_t block_size);
void	*_heapBM_alloc(_heapBM_t *heap, size_t size);
void	_heapBM_free(_heapBM_t *heap, void *ptr);

#endif // _HEAPBMMALLOC_H_

