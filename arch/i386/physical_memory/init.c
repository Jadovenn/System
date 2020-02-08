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

#include "cpu/cr.h"
#include "arch/physical_memory.h"
#include "arch/memlayout.h"
#include "arch/paging.h"

/**
 * Physical Memory Init procedure
 * 1. read physical memory region from multiboot header
 * 2. map physical aera after kernel to store bitset for the physical memory manager (pmm)
 * 3. map already taken region like kernel space and pmm biset map
 */

uint32_t _physical_mmap_start = 0;
uint32_t _physical_mmap_end = 0;

static void	__display_physical_memory_regions(multiboot_mmap_region_t *mmap) {
	printk("    addr: %#x, len: %u bytes, type: %d\n", (uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
}

static void	__vmap_physical_memory_region_groupe(multiboot_mmap_region_t *mmap) {
	if (!mmap->addr || mmap->type != 1) {
		return;
	}
	if (!(uint32_t)mmap->addr && (uint64_t)mmap->addr) {
		printk("PAE not supported, physical region ignored\n");
		return;
	}
	// Compute region's size in Kib
	size_t page_nb = mmap->len / 1024; // phys map in Kib
	page_nb /= 4; // nb of phys page 4kib
	// Compute nb of page available in this region
	page_nb = page_nb - (page_nb % 32); // align on 32
	size_t bitset_len = page_nb / 32;
	size_t size = bitset_len * 4 + sizeof(pmm_region_t);
	size_t pages = size / 0x1000;
	if (pages % 0x1000 || !pages) {
		pages += 1;
	}
	pmm_region_t *start = (pmm_region_t*)PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)_physical_mmap_end);
	uint32_t paddr = _physical_mmap_end;
	uint32_t vaddr = PHYSICAL_ADDR_TO_VIRTUAL(_physical_mmap_end);
	for (size_t nb_page = 0; nb_page < pages; nb_page += 1) {
		if (pg_map_physical(paddr, vaddr, 0x3, false) == EXIT_FAILURE) {
			PANIC("Could not map safly physical page frame");
			return;
		}
		paddr += 0x1000;
		vaddr += 0x1000;
	}
	_physical_mmap_end = paddr;
	memset(start, 0, pages * 0x1000);
	start->physical_addr = mmap->addr;
	start->page_nb = page_nb;
	start->bitset = (uint32_t*)((uint32_t)start + sizeof(pmm_region_t));
	start->next = NULL;
	if (physical_memory_map) {
		pmm_region_t *idx = physical_memory_map;
		while (idx->next) {
			idx = idx->next;
		}
		idx->next = start;
	}
	else {
		physical_memory_map = start;
	}
}

static void __display_usable_physical_memory() {
	size_t	size = 0;
	for (pmm_region_t *idx = physical_memory_map;
			idx; idx = idx->next) {
		printk("[%#x - %#x] %d Kib of usable memory\n",
				idx->physical_addr,
				idx->physical_addr + idx->page_nb * 0x1000,
				idx->page_nb*4);
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

static __inline void __map_kernel_physical_region() {
	uint32_t kernel_start_addr = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_kernel_start);
	uint32_t kernel_end_addr = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_end);
	kernel_end_addr -= kernel_end_addr % 0x1000;
	uint32_t result = pmm_set_region(kernel_start_addr, kernel_end_addr, true);
	if (result != EXIT_SUCCESS) {
		PANIC("Could not map physical aera of the kernel");
	}
	result = pmm_set_region(_physical_mmap_start, _physical_mmap_end - 0x1000, true);
	if (result != EXIT_SUCCESS) {
		PANIC("Could not map physical aera of the bitset for physical memory manager");
	}
}

void	physical_memory_init(multiboot_info *header) {
	printk("Physical Memory Regions:\n");
	mltb_foreach_physical_memory_region(header, &__display_physical_memory_regions);
	_physical_mmap_start = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_end);
	if (_physical_mmap_start % 0x1000) {
		_physical_mmap_start += (0x1000 - (_physical_mmap_start % 0x1000));
	}
	_physical_mmap_end = _physical_mmap_start;
	mltb_foreach_physical_memory_region(header, &__vmap_physical_memory_region_groupe);
	__display_usable_physical_memory();
	__map_kernel_physical_region();
}

