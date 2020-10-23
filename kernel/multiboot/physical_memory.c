/**
 * physical_memory_map.c - Multiboot Physical Memory Map
 * System sources under license MIT
 */

#include <stdint.h>

#include <arch/memlayout.h>

#include <multiboot.h>

void mltb_foreach_physical_memory_region(
		multiboot_info* header, void (*function)(multiboot_mmap_region_t*)) {
	multiboot_mmap_region_t* mmap_region;
	for (mmap_region =
	         (multiboot_mmap_region_t*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
	     (uint32_t)mmap_region <
	     PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
	     mmap_region = (multiboot_mmap_region_t*)((uint32_t)mmap_region +
	                                              mmap_region->size +
	                                              sizeof(mmap_region->size))) {
		function(mmap_region);
	}
}
