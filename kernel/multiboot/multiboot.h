/**
 * multiboot.h - Multiboot header
 * System sources under license MIT
 */

#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

#include <multiboot_types.h>

/**
 * @brief iterate over physical memory region
 * @param header - multiboot header
 * @param function - function that receive the region info
 */
void mltb_foreach_physical_memory_region(
		multiboot_info* header, void (*function)(multiboot_mmap_region_t*));

#endif // MULTIBOOT_H_
