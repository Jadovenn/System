/**
 * multiboot.h - Multiboot header
 * System sources under license MIT
 */

#ifndef MULTIBOOT_H_
# define MULTIBOOT_H_

#include <multiboot_types.h>

/**
 * @brief hold physical memory size
 * @attribute size - physical memory size in bytes
 * @attribute region_nb - number of available region
 */
typedef struct	mltb_available_physical_mem_size {
	size_t	size;
	size_t	region_nb;
}		mltb_available_physical_mem_size_t;


/**
 * @brief return size of usable physical memory
 * @return {mltb_available_physical_mem_sizet}
 */
mltb_available_physical_mem_size_t
mltb_available_physical_memory_size(multiboot_info *header);

/**
 * @brief iterate over physical memory region
 * @param header - multiboot header
 * @param function - function that recive the region info
 */
void
mltb_foreach_physical_memory_region(multiboot_info *header, void(*function)(multiboot_mmap_region_t*));

#endif // MULTIBOOT_H_

