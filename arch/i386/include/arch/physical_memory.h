/**
 * pmemory.h - Physical Memory Management
 * System sources under license MIT
 */

#ifndef PHYSICAL_MEMORY_H_
# define PHYSICAL_MEMORY_H_

#include <stdint.h>
#include <stdbool.h>

// physical memory region
typedef struct pmm_region {
	uint32_t	physical_addr;
	uint32_t	next_free_offset;
	size_t		page_nb;
	uint32_t	*bitset;
	struct 		pmm_region *next;
}		pmm_region_t;

extern pmm_region_t *physical_memory_map;

/**
 * @brief free a physical page
 * @param addr - address to the page to free
 * @details the physical page should not be used anymore
 * the free is performed via an offset computation so it is
 * pretty fast close to O(1)
 */
void	pmm_free(void *addr);

/**
 * @brief allocate one physical page
 * @return physical address to the allocated physical page
 * @details the allocator is implemented as a sequential search O(N)
 */
void	*pmm_alloc(void);

/**
 * @brief set value to physical page
 * @param p_addr - aligned physcial page's addr
 * @param value - value to set true/false
 * @return EXIT_SUCCESS/FAILURE
 */
uint32_t pmm_set_page(uint32_t p_addr, bool value);

/**
 * @brief set value to physical region including p_end_addr
 * @param p_start_addr - first page addr to map aligned
 * @param p_end_addr - last including page to map aligned
 * @param value - value to set true/false
 * @return EXIT_SUCCESS/FAILURE
 */
uint32_t pmm_set_region(const uint32_t p_start_addr, const uint32_t p_end_addr, bool value);

#endif // PHYSICAL_MEMORY_H_

