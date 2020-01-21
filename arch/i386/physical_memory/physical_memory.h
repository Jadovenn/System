/**
 * pmemory.h - Physical Memory Management
 * System sources under license MIT
 */

#ifndef PHYSICAL_MEMORY_H_
# define PHYSICAL_MEMORY_H_


typedef struct physical_mm_group {
	uint32_t	physical_addr;
	uint32_t	next_free_offset;
	size_t		page_nb;
	uint32_t	*bitset;
	struct 		physical_mm_group *next;
}		physical_mm_group_t;

extern physical_mm_group_t *physical_mmap;

void	physical_free(void);

void	*physical_alloc(void);

#endif // PHYSICAL_MEMORY_H_

