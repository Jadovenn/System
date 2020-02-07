/**
 * mmu_init.c - Memory Management Unit Installation
 * System under license MIT
 */

#include <stdlib.h>
#include <string.h>
#include <system.h>
#include <multiboot.h>
#include <kernel/stdio.h>
#include <kernel/heap.h>
#include <kernel/panic.h>

#include "cpu/mmu.h"
#include "cpu/cr.h"
#include "arch/paging.h"
#include "arch/memlayout.h"
#include "arch/hal.h"

/**
 * Step to initialize paging
 * 1. install boot page directory and page table
 * 2. mark .text section of the kernel as read-only
 * 3. mark .rodata section of the kernel as read-only
 * 4. flush the TLB to make change effective
 * 5. setup necessarly free space right after the kernel
 */

//
// Post boot - Pre init sequence
//

static void	__set_section_text_ro() {
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_code);
	uint32_t vaddr = (uint32_t)&_kernel_start;
	uint32_t paddr = VIRTUAL_ADDR_TO_PHYSICAL(&_kernel_start);
	unsigned page_mapped = 0;
	for (;paddr < text_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		paging_map_physical(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

static void	__set_section_rodata_ro() {
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_rodata);
	uint32_t vaddr = (uint32_t)&_rodata;
	uint32_t paddr = VIRTUAL_ADDR_TO_PHYSICAL(&_rodata);
	unsigned page_mapped = 0;
	for (;paddr < rodata_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		paging_map_physical(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

void	boot_paging_init() {
	mmu.boot_page_directory = &boot_page_directory;
	mmu.boot_page_table = &boot_page_table;
	__set_section_text_ro();
	__set_section_rodata_ro();
	flush_tlb();
}

/**
 * TODO:
 * 	1. install page directory
 *	1. set correct r/w access for text section
 *	2. set correct r/w access for rodata section
 *
 * func to write:
 * get_physical_addr_from_virtual() -> lookup physical addr in virtual table
 */

void	paging_init(multiboot_info *header) {
	(void)header;
	hal.mmu.page_directory = &boot_page_directory;
	__set_section_text_ro();
	__set_section_rodata_ro();
}

