/**
 * init.c - Physical Memory Layout Initialization
 * System sources under license MIT
 */

#include <stdint>
#include <stddef>

#include "physical_memory.h"

/**
 * 1. Alloc a static region for physical memory bitset
 * 2. Map physical Memory
 */

struct physical_memory_group	*groups = NULL;

static void	__read_multiboot_info_memory_map(multiboot_info *header) {
	printk("mmap_addr %#x\n", header->mmap_addr);
	printk("mmap_length %#x\n", header->mmap_length);
	multiboot_mmap_info *mmap;
	for (mmap = (multiboot_mmap_info*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap = (multiboot_mmap_info*)((uint32_t) mmap + mmap->size + sizeof(mmap->size)))
	{
		int result = map_frame_region((uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
		if (result == EXIT_FAILURE) {
			printk("Warning memory region not mapped\n");
		}
	}
}

static void	__add_physical_group() {

}

void	physical_memory_init() {

}

