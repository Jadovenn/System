/**
 * pmemory.h - Physical Memory Management
 * System sources under license MIT
 */

#ifndef PHYSICAL_MEMORY_H_
# define PHYSICAL_MEMORY_H_

extern struct physical_memory_group *physical_mmap;

typedef struct physical_mm_group {
	void		*start_addr;
	uint32_t	next_free_offset;
	uint32_t	*bitset;
	struct 		physical_memory_group *next;
}		physical_mm_group_t;

void	physical_free(void);

void	*physical_alloc(void);

#endif // PHYSICAL_MEMORY_H_

