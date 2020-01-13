/**
 * init.c - Physical Memory Layout Initialization
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <kernel/stdio.h>
#include <kernel/multiboot.h>

#include "physical_memory.h"
#include "cpu/mmu.h"
#include "cpu/cr.h"
#include "arch/memlayout.h"

/**
 * 1. Allocate groups at the end of the kernel region
 * 2. Read physical memory blocks, and add them as groups
 * 3. Map physical memory
 */

struct physical_memory_group	*physical_mmap = NULL;

static size_t	__physical_size(multiboot_info	*header) {
	multiboot_mmap_info	*mmap_info;
	size_t			size = 0;
	for (mmap_info = (multiboot_mmap_info*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap_info < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap_info = (multiboot_mmap_info*)((uint32_t) mmap_info + mmap_info->size + sizeof(mmap_info->size)))
	{
		if (mmap_info->type != 1)
			continue;
		size += sizeof(physical_mm_group_t);
		size += (mmap_info->len / 4) / 32; // number of pages devided by the number of bit in a single unit of the bitset
	}
	return size;
}

static void	*__vmap_physical_group_space(uint32_t placement_addr, size_t physical_group_size) {
	void	*mmap_ptr = (void*)PHYSICAL_PTR_TO_VIRTUAL(placement_addr);
	uint32_t page_offset = (uint32_t)mmap_ptr / 0x1000;
	uint32_t physical_addr = placement_addr;
	size_t size_to_map = physical_group_size;
	while (size_to_map > 0) {
		uint32_t entry = physical_addr & 0xFFFFF000;
		entry |= 0x003;
		mmu.boot_page_table[page_offset] = entry; // set right access to this page
		physical_addr += 0x1000;
		size_to_map -= 0x1000;
		page_offset = 1;
	}
	flush_tlb();
	return mmap_ptr;
}

static void	__read_multiboot_info_memory_map(multiboot_info *header) {
	printk("mmap_addr %#x\n", header->mmap_addr);
	printk("mmap_length %#x\n", header->mmap_length);
	printk("Physical Memory Map:\n");
	multiboot_mmap_info *mmap = NULL;
	for (mmap = (multiboot_mmap_info*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap = (multiboot_mmap_info*)((uint32_t) mmap + mmap->size + sizeof(mmap->size)))
	{
		printk("addr: %#x len: %dKib type: %d\n", (uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
	}
}

void	physical_memory_init(multiboot_info *header) {
	uint32_t placement = VIRTUAL_ADDR_TO_PHYSICAL(&_end);
	if (placement % 0x1000 != 0) { // align placement add to the next available page
		placement += (0x1000 - (placement % 0x1000));
	}
	__read_multiboot_info_memory_map(header);
	size_t physical_group_size = __physical_memory_size(header);
	printk("physical_memory_group size: %d\n", sizeof(physical_mm_group_t));
	printk("Sizeof physical memory block: %dKib\n", physical_group_size);
	physical_mmap = __vmap_physical_group_space(placement, 1024);
	// now try to write at mmap and see what happen
	//mmap->bitset = NULL;
	//mmap->next = NULL;
}

