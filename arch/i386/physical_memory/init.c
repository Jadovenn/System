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

#include "physical_memory.h"
#include "cpu/mmu.h"
#include "cpu/cr.h"
#include "arch/memlayout.h"
#include "arch/paging.h"

/**
 * 1. detecting useable physical memory
 * 2. map space for physical memory bitset
 * 3. then physical alloc/free should work
 */

physical_mm_group_t	*physical_mmap = NULL;
uint32_t _physical_mmap_start = 0;
uint32_t _physical_mmap_end = 0;

static void	__display_physical_memory_regions(multiboot_mmap_region_t *mmap) {
	printk("    addr: %#x, len: %d bytes, type: %d\n", (uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
}

static void	__vmap_physical_memory_region_groupe(multiboot_mmap_region_t *mmap) {
	if (mmap->addr == 0 || mmap->type != 1) {
		return;
	}
	// Compute size in Kib
	size_t page_nb = mmap->len / 1024; // phys map in Kib
	page_nb /= 4; // nb of phys page 4kib
	page_nb = page_nb - (page_nb % 32); // align on 32
	size_t bitset_len = page_nb / 32;
	size_t size = bitset_len * 4 + sizeof(physical_mm_group_t);
	size_t pages = size / 0x1000;
	if (pages % 0x1000) {
		pages += 1;
	}
	// Compute nb of page
	physical_mm_group_t *start = (physical_mm_group_t*)PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)_physical_mmap_end);
	uint32_t paddr = _physical_mmap_end;
	uint32_t vaddr = PHYSICAL_ADDR_TO_VIRTUAL(_physical_mmap_end);
	for (size_t nb_page = 0; nb_page < pages; nb_page += 1) {
		if (paging_map_physical(paddr, vaddr, 0x3, false) == EXIT_FAILURE) {
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
	start->bitset = (uint32_t*)((uint32_t)start + sizeof(physical_mm_group_t));
	start->next = NULL;
	if (physical_mmap) {
		physical_mm_group_t *idx = physical_mmap;
		while (idx->next) {
			idx = idx->next;
		}
		idx->next = start;
	}
	else {
		physical_mmap = start;
	}
}

static void __display_usable_physical_memory() {
	size_t	size = 0;
	for (physical_mm_group_t *idx = physical_mmap;
			idx; idx = idx->next) {
		size += idx->page_nb * 4;
	}
	size /= 1024;
	size_t size_gb = size / 1024;
	if (size_gb != 0) {
		printk("total usable physical memory: %d Gib and %d Mib\n", size_gb, size % 1024);
	}
	else {
		printk("total usable physical memory: %d Mib\n", size);
	}
}

void	physical_memory_init(multiboot_info *header) {
	printk("Physical Memory Regions:\n");
	mltb_foreach_physical_memory_region(header, &__display_physical_memory_regions);
	mltb_available_physical_mem_size_t	physmem_size = mltb_available_physical_memory_size(header);
	printk("Sizeof physical memory block: %dKib\n", physmem_size.size);
	printk("Number of physical memory region(s): %d\n", physmem_size.region_nb);
	_physical_mmap_start = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&_end);
	if (_physical_mmap_start % 0x1000 != 0) {
		_physical_mmap_start += (0x1000 - (_physical_mmap_start % 0x1000));
	}
	_physical_mmap_end = _physical_mmap_start;
	mltb_foreach_physical_memory_region(header, &__vmap_physical_memory_region_groupe);
	__display_usable_physical_memory();
}

