/**
 * pmemory.h - Physical Memory Management
 * System sources under license MIT
 */

#ifndef PHYSICAL_MEMORY_H_
# define PHYSICAL_MEMORY_H_

// physical memory region
typedef struct pm_region {
	uint32_t	physical_addr;
	uint32_t	next_free_offset;
	size_t		page_nb;
	uint32_t	*bitset;
	struct 		pm_region *next;
}		pm_region_t;

extern pm_region_t *physical_mmap;

void	physical_free(void);

void	*physical_alloc(void);

uint32_t	pmark_page(uint32_t p_addr);

#endif // PHYSICAL_MEMORY_H_

