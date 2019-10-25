/**
 * heap.c - Virtual Memory Allocation Management
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/panic.h>
#include <kernel/heap.h>

typedef struct heap_block_header_t {
	struct heap_block_header_t	*next;
	struct heap_block_header_t	*prev;
	void				*physical_addr;
	size_t				size;
	bool				is_free;
}		heap_block_header;

heap_block_header	*virtual_memory = NULL;

void	install_heap(void *placement_addr, void *physical_addr, size_t size) {
	virtual_memory = placement_addr;
	virtual_memory->next = NULL;
	virtual_memory->prev = NULL;
	virtual_memory->size = size - sizeof(heap_block_header);
	virtual_memory->is_free = true;
	virtual_memory->physical_addr = physical_addr + sizeof(heap_block_header);
}

/**
 * @brief fragment one block in two blocks
 * @return struct heap_block - new fragmented block or if it is not
 * possible to fragment the block, it return the initial block
 */
static heap_block_header	*_fragment_block(heap_block_header *block, size_t buddy_size) {
	heap_block_header	*buddy = NULL;
	size_t	buddy_block_size = buddy_size + sizeof(heap_block_header);
	uint32_t ptr = (uint32_t)block;
	if (block->size <= buddy_block_size + 1) { // Special Case: useless fragmentation, just return the block addr
		return block;
	}
	ptr += buddy_block_size; 
	buddy = (heap_block_header*)ptr;
	buddy->physical_addr = block->physical_addr + buddy_block_size;
	buddy->size = block->size - buddy_block_size;
	block->size = buddy_size; 
	buddy->is_free = true;
	buddy->next = block->next;
	buddy->prev = block;
	if (block->next) {
		block->next->prev = buddy;
	}
	block->next = buddy;
	return block;
}

/**
 * @biref merge a block and his buddy
 * @details the block merge with is right buddy, if the block is already
 * the size of the physical page, then it is not merged
 * @param heap_block_header - block to merge with is buddy on the next
 * @return heap_block_header - the new merged block
 */
static heap_block_header	*_merge_block(heap_block_header *block) {
	heap_block_header	*buddy = block->next;
	if (!block->is_free && !buddy) { // Special Case: block is not free or/and as no buddy
		return block;
	}
	if (!buddy->is_free) { // Special Case: buddy is not free
		return block;
	}
	// Special Case: this block are not buddy, because merge them result
	// in a block bigger than a physical page size
	if ((block->size + buddy->size) + (2 * sizeof(heap_block_header)) > get_page_size()) {
		return block;
	}
	block->size += buddy->size + sizeof(heap_block_header);
	block->next = buddy->next;
	if (buddy->next) {
		buddy->next->prev = block;
	}
	return block;
}

void *kmalloc(size_t block_size) {
	return kmalloc_physical(block_size, NULL);
}

/**
 * @brief kernel dynamic allocation
 * @attention block bigger than page size are not provided
 * @arg size_t - block_size desire size for 
 * @return void* - return valide pointer or NULL
 */
void	*kmalloc_physical(size_t block_size, uint32_t *physical_addr) {
	heap_block_header *iterator = virtual_memory;
	heap_block_header *block = NULL;
	if (block_size % 4 != 0) { // Special Case: requested size is not 4 bytes aligned
		block_size += 4 - block_size % 4;
	}
	while (!block) { // Regular Case: until a block is found
		if (!iterator) { // Special Case: no free block, then return NULL
			return NULL;
		}
		if (iterator->is_free) { // Regular Case: a free block is found
			if (iterator->size >= block_size) { // Regular Case: the free block as a good size commit it
				block = iterator;
			}
			else { // Special Case: block is to small, try to merge it with his buddy if it is free
				if (iterator->prev && iterator->prev->is_free) {
					iterator = _merge_block(iterator->prev);
					if (iterator->size >= block_size) {
						block = iterator;
					}
				}
			}
		}
		iterator = iterator->next;
	}
	if (block->size > block_size) { // Special Case: the block is bigger than what we need try to fragment it
		block = _fragment_block(block, block_size);
	}
	block->is_free = false;
	if (physical_addr) {
		*physical_addr = block->physical_addr + sizeof(heap_block_header);
	}
	return (void*)(((uint32_t)block) + (sizeof(heap_block_header)));
} // O(n) = n, where n is the number of blocks

// TODO: Kfree could be done in O(n) = 1, add a magic number in heap_block_header

/**
 * @brief mark the ptr's related block as free
 * @details if the pointer is not related to a block, a kernel PANIC is
 * raised. The behavior may change in futur. This allow dev to avoid leaks
 * if they provide a wrong addr. But it not prevent leaks if blocks is never free at all.
 * @param void* - ptr to match with a block and mark as free
 */
void	kfree(void *ptr) {
	uint32_t block_ptr = ((uint32_t)ptr) - (sizeof(heap_block_header));
	heap_block_header *iterator = virtual_memory;
	heap_block_header *block = NULL;
	while (!block) {
		if (!iterator) { // Special Case: Ptr does not relate to a block, panic to let dev correct theirs errors
			PANIC("kfree, %#x does not reffer to any blocks", ptr);
		}
		if ((uint32_t)iterator == block_ptr) { // Regular Case: block found
			block = iterator;
		}
		iterator = iterator->next;
	}
	block->is_free = true;
	_merge_block(block);
} // O(n) = n, where n is the number of block

