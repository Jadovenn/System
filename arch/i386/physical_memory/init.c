/**
 * init.c - Physical Memory Layout Initialization
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <kernel/stdio.h>
#include <kernel/multiboot.h>

#include "arch/memlayout.h"
#include "physical_memory.h"

/**
 * 1. Allocate groups at the end of the kernel region
 * 2. Read physical memory blocks, and add them as groups
 * 3. Map physical memory
 */

struct physical_memory_group	*groups = NULL;

static void	__read_multiboot_info_memory_map(multiboot_info *header) {
	printk("mmap_addr %#x\n", header->mmap_addr);
	printk("mmap_length %#x\n", header->mmap_length);
	printk("Physical Memory Map:\n");
	multiboot_mmap_info *mmap;
	for (mmap = (multiboot_mmap_info*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap = (multiboot_mmap_info*)((uint32_t) mmap + mmap->size + sizeof(mmap->size)))
	{
		printk("addr: %#x len: %#x type: %d\n", (uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
	}
}

void	physical_memory_init(multiboot_info *header) {
	uint32_t placement = VIRTUAL_ADDR_TO_PHYSICAL(&_end);
	if (placement % 0x1000 != 0) {
		placement += (0x1000 - (placement % 0x1000));
	}
	groups = (void*)PHYSICAL_PTR_TO_VIRTUAL(placement);
	__read_multiboot_info_memory_map(header);
}

