/**
 * init.c - Physical Memory Layout Initialization
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <multiboot.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>

#include <cpu/cr.h>
#include <arch/physical_memory.h>
#include <arch/memlayout.h>
#include <arch/paging.h>

/**
 * Physical Memory Init procedure
 * 1. read physical memory region from multiboot header
 * 2. map physical aera after kernel to store bitset for the physical memory manager (pmm)
 * 3. map already taken region like kernel space and pmm biset map
 */

uint32_t _physical_mmap_start = 0;
uint32_t _physical_mmap_end = 0;

static void	__create_physical_region(uint32_t len, uint32_t addr, uint32_t type) {
	size_t bytes = sizeof(pmm_region_t);
	size_t usable_page_count = 0;
	if (type == MEMORY_AVAILABLE || type == MEMORY_LOWER_REGION) {
		size_t page_count = (len / 1024) / 4;
		usable_page_count = page_count - (page_count % 32);
		bytes += usable_page_count / 8;
	}
	size_t page_count = (bytes / 0x1000 ? bytes / 0x1000 + 1 : 1);
	size_t available_bytes = 0x1000 - _physical_mmap_end % 0x1000;
	if (bytes > available_bytes) {
		uintptr_t paddr = _physical_mmap_end + (0x1000 - _physical_mmap_end % 0x1000);
		uintptr_t vaddr = PHYSICAL_ADDR_TO_VIRTUAL(paddr);
		for (size_t nb_page = 0; nb_page < page_count; nb_page += 1) {
			if (pg_map(paddr, vaddr, 0x3, false) == EXIT_FAILURE) {
				PANIC("Fatal error, could not map physical region, more RAM needed");
			}
			paddr += 0x1000;
			vaddr += 0x1000;
		}
	}
	pmm_region_t *new_region = (pmm_region_t*)PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)_physical_mmap_end);
	_physical_mmap_end += bytes + 1;
	memset(new_region, 0, bytes);
	new_region->pstart = addr;
	new_region->pend = (addr + len - 1); 
	new_region->bytes = len;
	new_region->page_nb = usable_page_count;
	new_region->type = type;
	new_region->bitset = NULL;
	new_region->next = NULL;
	if (type == MEMORY_AVAILABLE || type == MEMORY_LOWER_REGION) {
		new_region->bitset = (uint32_t*)&new_region[1];
	}
	if (!physical_memory_map) {
		physical_memory_map = new_region;
		return;
	}
	pmm_region_t *region = physical_memory_map;
	for (; region->next; region = region->next);
	region->next = new_region;
}

static void	__read_physical_memory_regions_block(multiboot_mmap_region_t *mmap) {
	if (!(uint32_t)mmap->addr && (uint64_t)mmap->addr) {
		printk("PAE not supported, physical region ignored\n");
		return;
	}
	if (mmap->addr < 0x100000 && mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
		__create_physical_region(mmap->len, mmap->addr, MEMORY_BIOS);
	}
	else if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE
			&& mmap->addr + mmap->len - 1 < 0x1000000) {
		__create_physical_region(mmap->len, mmap->addr, MEMORY_AVAILABLE);
	}
	else if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE
			&& mmap->addr < 0x1000000 && mmap->addr + mmap->len - 1 > 0x1000000) {
		__create_physical_region(0x1000000 - mmap->addr, mmap->addr, MEMORY_LOWER_REGION);
		__create_physical_region(mmap->len - (0x1000000 - mmap->addr),
						mmap->addr + (0x1000000 - mmap->addr), mmap->type);
	}
	else {
		__create_physical_region(mmap->len, mmap->addr, mmap->type);
	}
}

static __inline void __display_usable_physical_memory() {
	size_t	size = 0;
	printk("Physical Memory Regions Map:\n");
	for (pmm_region_t *idx = physical_memory_map;
			idx; idx = idx->next) {
		printk("   [%#x : %#x] type: %d, %d Kib of usable memory\n",
				idx->pstart,
				idx->pend,
				idx->type,
				idx->page_nb*4);
		if (idx->type != MEMORY_AVAILABLE && idx->type != MEMORY_LOWER_REGION) {
			continue;
		}
		size += idx->page_nb * 4;
	}
	size /= 1024;
	size_t size_gb = size / 1024;
	if (size_gb != 0) {
		printk("Total available RAM: %d Gib and %d Mib\n", size_gb, size % 1024);
	}
	else {
		printk("Total available RAM: %d Mib\n", size);
	}
}

static __inline void __map_used_physical_region() {
	uint32_t kernel_start_addr = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_kernel_start);
	uint32_t kernel_end_addr = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_end);
	kernel_end_addr -= kernel_end_addr % 0x1000;
	uint32_t result = pmm_set_region(kernel_start_addr, kernel_end_addr, PM_PAGE_PRESENT);
	if (result != EXIT_SUCCESS) {
		PANIC("Could not map physical aera of the kernel");
	}
	result = pmm_set_region(_physical_mmap_start, _physical_mmap_end - 0x1000, PM_PAGE_PRESENT);
	if (result != EXIT_SUCCESS) {
		PANIC("Could not map physical aera of the bitset for physical memory manager");
	}
}

void	physical_memory_init(multiboot_info *header) {
	_physical_mmap_start = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_end);
	if (_physical_mmap_start % 0x1000) {
		_physical_mmap_start += (0x1000 - (_physical_mmap_start % 0x1000));
	}
	_physical_mmap_end = _physical_mmap_start;
	if (pg_map(_physical_mmap_start, PHYSICAL_ADDR_TO_VIRTUAL(_physical_mmap_start), 0x3, false) == EXIT_FAILURE) {
		PANIC("Fatal error, could not map physical region, more RAM needed");
	}
	mltb_foreach_physical_memory_region(header, &__read_physical_memory_regions_block);
	__display_usable_physical_memory();
	__map_used_physical_region();
}

