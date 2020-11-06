/**
 * malloc.c - General purpose allocator
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <hal/memory.h>
#include <kernel/panic.h>

struct Heap_pool;

// Block structure
typedef struct Heap_block {
	struct Heap_pool*  pool;
	struct Heap_block* next;
	struct Heap_block* prev;
} Heap_block_t;

// Pool structure
typedef struct Heap_pool {
	unsigned           boundary;
	struct Heap_pool*  nextPool;
	struct Heap_pool*  prevPool;
	struct Heap_pool*  previousAllocatedPool;
	struct Heap_block* freeBlocks;
	struct Heap_block* usedBlocks;
} Heap_pool_t;

static Heap_pool_t* S_Heap      = NULL;
static void*        S_Break     = NULL;
static int          S_Page_size = -1;

#define MIN_POOL_BOUNDARY 0x10
#define MAX_POOL_BOUNDARY (size_t) S_Page_size
#define DEFAULT_POOL_SIZE 0x8000 // 32Kib

/********************************
 ** Heap private function      **
 ********************************/

static Heap_pool_t* _create_pool(size_t aBoundary) {
	Heap_pool_t* pool  = NULL;
	// The default pool size is 32Kib
	size_t pageAligned = DEFAULT_POOL_SIZE;

	// If the request is more than the boundary max create a standalone pool
	if (aBoundary > MAX_POOL_BOUNDARY) {
		size_t poolSize    = aBoundary + sizeof(Heap_pool_t) + sizeof(Heap_block_t);
		pageAligned = poolSize + (S_Page_size - (poolSize % S_Page_size));
	}

	// Increase the heap break
	pool = Hal_sbrk(pageAligned);
	if (pool == NULL) {
		PANIC("Malloc ::: Not enough memory");
	}

	pool->boundary              = aBoundary;
	pool->previousAllocatedPool = S_Break;
	pool->usedBlocks            = NULL;
	S_Break                     = pool;

	// Initialize the first pool's block
	Heap_block_t* block = (void*)((uintptr_t)pool + sizeof(Heap_pool_t));
	pool->freeBlocks    = block;
	block->pool         = pool;
	block->prev         = NULL;
	block->next         = NULL;
	// If the pool is a regular one initialize all other blocks
	if (aBoundary <= MAX_POOL_BOUNDARY) {
		uintptr_t currentBreak = (uintptr_t)Hal_sbrk(0);
		uintptr_t currentAddr =
				((uintptr_t)block + sizeof(Heap_block_t) + aBoundary);

		while (currentBreak - currentAddr > aBoundary + sizeof(Heap_block_t)) {
			Heap_block_t* newBlock = (void*)currentAddr;
			block->next            = newBlock;
			newBlock->pool         = pool;
			newBlock->prev         = block;
			newBlock->next         = NULL;
			currentAddr = ((uintptr_t)block + sizeof(Heap_block_t) + aBoundary);
			block       = newBlock;
		}
	}

	// This pool is the first one
	if (S_Heap == NULL) {
		S_Heap = pool;
	} else {
		// Otherwise pools are ordered by boundary size
		Heap_pool_t* cursor = S_Heap;
		while (cursor->boundary <= pool->boundary) {
			if (!cursor->nextPool) {
				cursor->nextPool = pool;
				pool->prevPool   = cursor;
				pool->nextPool   = NULL;
				return pool;
			}
			cursor = cursor->nextPool;
		}
		if (cursor->prevPool) {
			cursor->prevPool->nextPool = pool;
		} else {
			S_Heap = pool;
		}
		pool->prevPool   = cursor->prevPool;
		cursor->prevPool = pool;
		pool->nextPool   = cursor;
	}
	return pool;
}

static void _try_delete_pool(Heap_pool_t* aPool) {
	if (!aPool) {
		return;
	}
	// If the pool has used block do nothing
	if (aPool->usedBlocks != NULL) {
		return;
	}
	// If the pool is the last recorded break delete it
	if (aPool == S_Break) {
		if (aPool->prevPool) {
			aPool->prevPool->nextPool = aPool->nextPool;
		}
		if (aPool->nextPool) {
			aPool->nextPool->prevPool = aPool->prevPool;
		}
		S_Break = aPool->previousAllocatedPool;
		if (Hal_brk(aPool) == EXIT_FAILURE) {
			PANIC("Malloc ::: corrupted free.");
		}
	}
	// Recursively try to delete the next pool if possible
	_try_delete_pool(S_Break);
}

/********************************
 ** Heap public init function  **
 ********************************/

void* malloc(size_t aSize) {
	// First block
	if (S_Heap == NULL) {
		S_Page_size = Hal_get_page_size();
		if (aSize > MAX_POOL_BOUNDARY) {
			Heap_pool_t* pool = _create_pool(aSize);
			pool->usedBlocks  = pool->freeBlocks;
			pool->freeBlocks  = NULL;
			return (void*)((uintptr_t)pool->usedBlocks + sizeof(Heap_block_t));
		}

		size_t request = MIN_POOL_BOUNDARY;
		while (aSize > request) {
			request = request << 1;
		}
		_create_pool(request);
	}

	// If request bigger > 32Kib create a standalone pool with one block
	if (aSize > MAX_POOL_BOUNDARY) {
		Heap_pool_t* pool = _create_pool(aSize);
		pool->usedBlocks  = pool->freeBlocks;
		pool->freeBlocks  = NULL;
		return (void*)((uintptr_t)pool->usedBlocks + sizeof(Heap_block_t));
	}

	// Align request on allocation boundary
	size_t request = 0x10;
	while (aSize > request) {
		request = request << 1;
	}

	// Find an appropriate pool
	Heap_pool_t* pool = S_Heap;
	while (pool->boundary != request) {
		pool = pool->nextPool;
		// If no pool were found, create a new one
		if (!pool) {
			pool = _create_pool(request);
		}
	}
	// If the selected pool does not have free blocks create a new one
	if (pool->freeBlocks == NULL) {
		pool = _create_pool(request);
	}
	// Use the first free block to complete the request
	Heap_block_t* block = pool->freeBlocks;
	pool->freeBlocks    = block->next;
	block->next         = pool->usedBlocks;
	block->prev         = NULL;
	pool->usedBlocks    = block;
	return (void*)((uintptr_t)block + sizeof(Heap_block_t));
}

void free(void* anAddr) {
	// Compute the block structure using the provided address
	Heap_block_t* block = (void*)((uintptr_t)anAddr - sizeof(Heap_block_t));
	Heap_pool_t*  pool  = block->pool;

	// The block is the first of the list
	if (block->prev == NULL) {
		pool->usedBlocks = block->next;
		if (block->next) {
			block->next->prev = NULL;
		}
	} else {
		// The block is not the first one
		block->prev->next = block->next;
		if (block->next) {
			block->next->prev = block->prev;
		}
		block->prev = NULL;
	}
	block->next      = pool->freeBlocks;
	pool->freeBlocks = block;

	// Try to delete the pool if possible
	_try_delete_pool(pool);
}
