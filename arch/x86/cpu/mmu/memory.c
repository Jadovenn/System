/**
 * memory.c - Virtual Memory Allocation Management
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/types.h>
#include <cpu/mmu.h>

struct heap_block_t {
	struct heap_block_t	*next;
	struct heap_block_t	*prev;
	size_t			size;
	bool			is_free;
};

struct heap_block_t	*virtual_memory = NULL;

/**
 * @details add a new block to the virtual memory list
 * @return struct heap_block_t - pointer to the new block
 */
static struct heap_block_t	*_add_new_block() {
	struct heap_block_t *new_block = (struct heap_block_t*)get_new_heap_page(); ;
	struct heap_block_t *iterator = virtual_memory;
	if (!new_block) {
		return NULL;
	}
	new_block->next = NULL;
	new_block->size = get_page_size() - sizeof(struct heap_block_t); 
	new_block->is_free = TRUE;
	if (iterator) {
		while (iterator->next)
			iterator = iterator->next;
		new_block->prev = iterator;
		iterator->next = new_block;
	}
	else {
		virtual_memory = new_block;
	}
	return new_block;
}

/**
 * @details fragment one block in two blocks
 * @return struct heap_block - new fragmented block or if it is not
 * possible to fragment the block, it return the initial block
 */
static struct heap_block_t	*_fragment_block(struct heap_block_t *block, size_t buddy_size) {
	struct	heap_block_t	*buddy = NULL;
	size_t	buddy_block_size = buddy_size + sizeof(struct heap_block_t);
	uint32_t ptr = (uint32_t)block;
	if (block->size <= buddy_block_size + 1) { // Special Case: useless fragmentation, just return the block addr
		return block;
	}
	ptr += buddy_block_size / 4; 
	buddy = (struct heap_block_t *)ptr;
	buddy->size = block->size - buddy_block_size;
	block->size = buddy_size; 
	buddy->is_free = TRUE;
	buddy->next = block->next;
	buddy->prev = block;
	if (block->next) {
		block->next->prev = buddy;
	}
	block->next = buddy;
	return block;
}

/**
 * @brief kernel dynamic allocation
 * @attention block bigger than page size are not provided
 * @arg size_t - block_size desire size for 
 * @return void* - return valide pointer or NULL
 */
void	*kmalloc(size_t block_size) {
	struct heap_block_t *iterator = virtual_memory;
	struct heap_block_t *block = NULL;
	if (block_size > 4096) { // Special Case: size is bigger than physical page size
		return NULL;
	}
	if (block_size % 4 != 0) { // Special Case:  block_size requested is not 4 bytes aligned
		block_size += block_size % 4;
	}
	while (!block) { // Regular Case: until a block is found
		if (!iterator) { // Special Case: No free block, then add new block
			iterator = _add_new_block();
			if (!iterator) { // Special Case: Heap overflow, no free heap page anymore
				return NULL;
			}
		}
		if (iterator->is_free) { // Regular Case: a free block is found
			if (iterator->size >= block_size) { // Regular Case: the free block as a good size return it
				block = iterator;
			}
		}
		iterator = iterator->next;
	}
	if (block->size > block_size) { // Special Case: the block is bigger try to fragment it
		block = _fragment_block(block, block_size);
	}
	block->is_free = FALSE;
	return (void*)(((uint32_t)block) + (sizeof(struct heap_block_t) / 4));
} // O(n) = n, where n is the number of blocks

// TODO: find why when a block is free and reallocate immediatly, is value is altered
// TODO: on free merge fragmented block if possible

/**
 * @brief mark the ptr's related block as free
 */
void	kfree(void *ptr) {
	uint32_t block_ptr = ((uint32_t)ptr) - (sizeof(struct heap_block_t) / 4);
	struct heap_block_t *iterator = virtual_memory;
	struct heap_block_t *block = NULL;
	while (!block) {
		if (!iterator) { // Special Case: Ptr does not relate to a block, panic to let dev correct theirs errors
			PANIC("kfree, %#x does not reffer to any blocks", ptr);
		}
		if ((uint32_t)iterator == block_ptr) { // Regular Case: block found
			block = iterator;
		}
		iterator = iterator->next;
	}
	block->is_free = TRUE;
}

