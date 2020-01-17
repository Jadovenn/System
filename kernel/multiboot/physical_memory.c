/**
 * physical_memory_map.c - Multiboot Physical Memory Map
 * System sources under license MIT
 */

#include <stdint.h>
#include <multiboot.h>
#include <arch/memlayout.h>

mltb_available_physical_mem_size_t	mltb_available_physical_memory_size(multiboot_info *header) {
	multiboot_mmap_region_t		*mmap_region;
	mltb_available_physical_mem_size_t	phys_mem;
	phys_mem.size = 0;
	phys_mem.region_nb = 0;
	for (mmap_region = (multiboot_mmap_region_t*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap_region < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap_region = (multiboot_mmap_region_t*)((uint32_t) mmap_region + mmap_region->size + sizeof(mmap_region->size)))
	{
		if (mmap_region->type != 1)
			continue;
		// number of pages devided by the number of bit in a single unit of the bitset
		phys_mem.size += (mmap_region->len / 4) / 32;
		phys_mem.region_nb += 1;
	}
	return phys_mem;
}

void	mltb_foreach_physical_memory_region(multiboot_info *header, void(*function)(multiboot_mmap_region_t*)) {
	multiboot_mmap_region_t	*mmap_region;
	for (mmap_region = (multiboot_mmap_region_t*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap_region < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap_region = (multiboot_mmap_region_t*)((uint32_t) mmap_region + mmap_region->size + sizeof(mmap_region->size)))
	{
		function(mmap_region);
	}
}

