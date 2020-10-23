/**
 * pmemory.h - Physical Memory Management
 * System sources under license MIT
 */

#ifndef PHYSICAL_MEMORY_H_
#define PHYSICAL_MEMORY_H_

#include <stdbool.h>
#include <stdint.h>

// physical memory region
typedef struct pmm_region {
	struct pmm_region* next;
	uintptr_t          pstart;
	uintptr_t          pend;
	uint32_t           bytes;
	size_t             page_nb;
	uint32_t           type;
	uint32_t*          bitset;
} pmm_region_t;

extern pmm_region_t* physical_memory_map;

/**
 * @brief free a physical page
 * @param addr - address to the page to free
 * @details the physical page should not be used anymore
 * the free is performed via an offset computation so it is
 * pretty fast, O(1)
 */
void pmm_release_pages(uintptr_t addr);

/**
 * @brief allocate one physical page
 * @param type - memory flage from api/mm.h
 * @return physical address to the allocated physical page
 * @details the allocator is implemented as a sequential search O(N)
 */
uintptr_t pmm_get_page(uint8_t type);

/**
 * @brief set value to physical page
 * @param p_addr - aligned physcial page's addr
 * @param value - value to set true/false
 * @return EXIT_SUCCESS/FAILURE
 */
uint32_t pmm_set_page(uintptr_t p_addr, bool value);

#define PM_PAGE_PRESENT     true
#define PM_PAGE_NOT_PRESENT false

/**
 * @brief set value to physical region including p_end_addr
 * @param p_start_addr - first page addr to map aligned
 * @param p_end_addr - last including page to map aligned
 * @param value - value to set true/false
 * @return EXIT_SUCCESS/FAILURE
 */
uint32_t pmm_set_region(const uintptr_t p_start_addr,
                        const uintptr_t p_end_addr,
                        bool            value);

#endif // PHYSICAL_MEMORY_H_
