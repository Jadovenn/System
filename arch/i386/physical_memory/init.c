/**
 * init.c - Physical Memory Layout Initialization
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <multiboot.h>
#include <kernel/stdio.h>

#include "physical_memory.h"
#include "cpu/mmu.h"
#include "cpu/cr.h"
#include "arch/memlayout.h"
#include "arch/paging.h"

/**
 * 1. Allocate groups at the end of the kernel region
 * 2. Read physical memory blocks, and add them as groups
 * 3. Map physical memory
 */

struct physical_memory_group	*physical_mmap = NULL;

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

static void	__display_physical_memory_regions(multiboot_mmap_region_t *mmap) {
	printk("    addr: %#x, len: %d bytes, type: %d\n", (uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
}

void	physical_memory_init(multiboot_info *header) {
	uint32_t physical_placement = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_end);
	if (physical_placement % 0x1000 != 0) { // align placement add to the next available page
		physical_placement += (0x1000 - (physical_placement % 0x1000));
	}
	uint32_t virtual_placement = PHYSICAL_ADDR_TO_VIRTUAL(physical_placement);
	printk("Physical Memory Regions:\n");
	mltb_foreach_physical_memory_region(header, &__display_physical_memory_regions);
	mltb_available_physical_mem_size_t	physmem_size = mltb_available_physical_memory_size(header);
	printk("Sizeof physical memory block: %dKib\n", physmem_size.size);
	printk("Number of physical memory region(s): %d\n", physmem_size.region_nb);
	paging_map_physical(physical_placement, virtual_placement, 0x3);
	//physical_mmap = __vmap_physical_group_space(placement, 1024);
	// now try to write at mmap and see what happen
	//mmap->bitset = NULL;
	//mmap->next = NULL;
}

