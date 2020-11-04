/**
 * init.c - i386 Memory Management API
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/multiboot.h>
#include <kernel/panic.h>
#include <kernel/stdio.h>

#include <arch/ld.h>
#include <arch/paging.h>
#include <cpu/cr.h>

#include "memory/boot/boot_memory.h"
#include "memory/physical.h"
#include "memory/virtual.h"

/**
 * Physical Memory Init procedure
 * 1. read physical memory region from multiboot header
 * 2. map physical area after kernel to store bitset for the physical memory
 * manager (pmm)
 * 3. map already taken region like kernel space and pmm bitset map
 */

/********************************
 **    Globals definition      **
 ********************************/

uint32_t G_Physical_mmap_start = 0;
uint32_t G_Physical_mmap_end   = 0;

/********************************
 **   Init static function     **
 ********************************/

static void
_create_physical_region(uint32_t len, uint32_t addr, uint32_t type) {
	size_t bytes             = sizeof(Physical_memory_region_t);
	size_t usable_page_count = 0;
	if (type == mt_AVAILABLE || type == mt_LOWER_REGION) {
		size_t page_count = (len / 1024) / 4;
		usable_page_count = page_count - (page_count % 32);
		bytes += usable_page_count / 8;
	}
	size_t page_count      = (bytes / 0x1000 ? bytes / 0x1000 + 1 : 1);
	size_t available_bytes = 0x1000 - G_Physical_mmap_end % 0x1000;
	if (bytes > available_bytes) {
		uintptr_t paddr =
				G_Physical_mmap_end + (0x1000 - G_Physical_mmap_end % 0x1000);
		uintptr_t vaddr = PHYSICAL_ADDR_TO_VIRTUAL(paddr);
		for (size_t nb_page = 0; nb_page < page_count; nb_page += 1) {
			if (Boot_paging_map(paddr, vaddr, 0x3, false) == EXIT_FAILURE) {
				PANIC("Fatal error, could not map physical region, more RAM needed");
			}
			paddr += 0x1000;
			vaddr += 0x1000;
		}
	}
	Physical_memory_region_t* new_region =
			(Physical_memory_region_t*)PHYSICAL_PTR_TO_VIRTUAL(
					(uint32_t*)G_Physical_mmap_end);
	G_Physical_mmap_end += bytes + 1;
	memset(new_region, 0, bytes);
	new_region->startAddr = addr;
	new_region->endAddr   = (addr + len - 1);
	new_region->bytes     = len;
	new_region->page_nb   = usable_page_count;
	new_region->type      = type;
	new_region->bitset    = NULL;
	new_region->next      = NULL;
	if (type == mt_AVAILABLE || type == mt_LOWER_REGION) {
		new_region->bitset = (uint32_t*)&new_region[1];
	}
	if (!G_Physical_memory_map) {
		G_Physical_memory_map = new_region;
		return;
	}
	Physical_memory_region_t* region = G_Physical_memory_map;
	for (; region->next; region = region->next)
		;
	region->next = new_region;
}

static void _read_physical_memory_regions_block(Multiboot_mmap_region_t* mmap) {
	if (!(uint32_t)mmap->addr && (uint64_t)mmap->addr) {
		printk("PAE not supported, physical region ignored\n");
		return;
	}
	if (mmap->addr < 0x100000 && mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
		_create_physical_region(mmap->len, mmap->addr, mt_BIOS);
	} else if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE &&
	           mmap->addr + mmap->len - 1 < 0x1000000) {
		_create_physical_region(mmap->len, mmap->addr, mt_AVAILABLE);
	} else if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE &&
	           mmap->addr < 0x1000000 && mmap->addr + mmap->len - 1 > 0x1000000) {
		_create_physical_region(0x1000000 - mmap->addr, mmap->addr,
		                        mt_LOWER_REGION);
		_create_physical_region(mmap->len - (0x1000000 - mmap->addr),
		                        mmap->addr + (0x1000000 - mmap->addr), mmap->type);
	} else {
		_create_physical_region(mmap->len, mmap->addr, mmap->type);
	}
}

static __inline void _display_usable_physical_memory() {
	size_t size = 0;
	printk("Physical Memory Regions:\n");
	for (Physical_memory_region_t* idx = G_Physical_memory_map; idx;
	     idx                           = idx->next) {
		printk("   [%#x : %#x] type: %d, %d Kib of usable memory\n", idx->startAddr,
		       idx->endAddr, idx->type, idx->page_nb * 4);
		if (idx->type != mt_AVAILABLE && idx->type != mt_LOWER_REGION) {
			continue;
		}
		size += idx->page_nb * 4;
	}
	size /= 1024;
	size_t size_gb = size / 1024;
	if (size_gb != 0) {
		printk("Total usable physical memory: %d Gib and %d Mib\n", size_gb,
		       size % 1024);
	} else {
		printk("Total usable physical memory: %d Mib\n", size);
	}
}

static __inline void _map_used_physical_region() {
	uint32_t kernel_start_addr =
			VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&G_Start_kernel);
	uint32_t kernel_end_addr = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&G_End_kernel);
	kernel_end_addr -= kernel_end_addr % 0x1000;
	uint32_t result = Physical_memory_set_region(kernel_start_addr,
	                                             kernel_end_addr, pms_PRESENT);
	if (result != EXIT_SUCCESS) {
		PANIC("Could not map physical area of the kernel");
	}
	result = Physical_memory_set_region(
			G_Physical_mmap_start, G_Physical_mmap_end - 0x1000, pms_PRESENT);
	if (result != EXIT_SUCCESS) {
		PANIC("Could not map physical area of the bitset for physical memory "
		      "manager");
	}
}

/********************************
 **      Public function       **
 ********************************/

void Init_physical_memory(uintptr_t aMultibootMmapAddr,
                          uint32_t  aMultibootMmapLength) {
	G_Physical_mmap_start = VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&G_End_kernel);
	if (G_Physical_mmap_start % 0x1000) {
		G_Physical_mmap_start += (0x1000 - (G_Physical_mmap_start % 0x1000));
	}
	G_Physical_mmap_end = G_Physical_mmap_start;
	if (Boot_paging_map(G_Physical_mmap_start,
	                    PHYSICAL_ADDR_TO_VIRTUAL(G_Physical_mmap_start), 0x3,
	                    false) == EXIT_FAILURE) {
		PANIC("Fatal error, could not map physical region, more RAM needed");
	}
	Multiboot_mmap_region_t* mmapRegion = NULL;
	for (mmapRegion = (Multiboot_mmap_region_t*)PHYSICAL_PTR_TO_VIRTUAL(
					 aMultibootMmapAddr);
	     (uint32_t)mmapRegion <
	     PHYSICAL_ADDR_TO_VIRTUAL(aMultibootMmapAddr) + aMultibootMmapLength;
	     mmapRegion =
	         (Multiboot_mmap_region_t*)((uint32_t)mmapRegion + mmapRegion->size +
	                                    sizeof(mmapRegion->size))) {
		_read_physical_memory_regions_block(mmapRegion);
	}
	_display_usable_physical_memory();
	_map_used_physical_region();
}
