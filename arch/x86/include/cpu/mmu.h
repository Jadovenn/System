/**
 * mmu.h - Memory Management Unit
 * System under license MIT
 */

#ifndef MMU_H_
# define MMU_H_

/**
 * @brief Memory Management Unit
 * @details mmu c object is responsible of free aera mapping, 
 * address translation and placement address
 */

#include <kernel/types.h>
#include <kernel/multiboot.h>

/**
 * @brief return the physical page size
 * @details it is accessible via the global object
 * but this is not guarantee to be the case in the future.
 * Prefer to use this getter instead.
 */
size_t	get_page_size();

void	*get_new_heap_page();

typedef struct 		kernel_heap_t {
	uint32_t	*base;
	uint32_t	*top;
	uint32_t	placement_address;
	size_t		page_count;
} 			kheap;

struct mmu_t	{
	uint32_t	*boot_page_directory;
	uint32_t	*boot_page_table;
	size_t		page_size;
	kheap		heap;
};

extern struct mmu_t	mmu;

#endif // MMU_H_

