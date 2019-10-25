/**
 * mmu_init.c - Memory Management Unit Installation
 * System under license MIT
 */

#include <stdlib.h>
#include <system.h>
#include <kernel/stdio.h>
#include <kernel/heap.h>
#include <kernel/panic.h>

#include "cpu/mmu.h"
#include "cpu/cr.h"
#include "arch/memlayout.h"

/**
 * Step to initialize paging
 * 1. install boot page directory and page table
 * 2. mark .text section of the kernel as read-only
 * 3. mark .rodata section of the kernel as read-only
 * 3. flush the TLB to make change effective
 */

static void	__set_section_text_ro() {
	uint32_t text_start_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_kernel_start);
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_code);
	uint32_t page_offset = text_start_addr / 0x1000;
	uint32_t pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	while (pte_addr < text_end_addr) {
		mmu.boot_page_table[page_offset] = pte_addr | 0x001;
		page_offset += 1;
		pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	}
}

static void	__set_section_rodata_ro() {
	uint32_t rodata_start_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_rodata);
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_rodata);
	uint32_t page_offset = rodata_start_addr / 0x1000;
	uint32_t pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000;
	while (pte_addr < rodata_end_addr) {
		mmu.boot_page_table[page_offset] = pte_addr | 0x001;
		page_offset += 1;
		pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	}
}

void	paging_init() {
	mmu.boot_page_directory = &boot_page_directory;
	mmu.boot_page_table = &boot_page_table;
	__set_section_text_ro();
	__set_section_rodata_ro();
	flush_tlb();
}

/**
 * legacy code, keep it here

 * @details configure kernel heap just after the physical
 * space of the kernel. But in virtual space the heap is before the kernel

static void	configure_kernel_heap() {
	const size_t page_size = get_page_size();
	mmu.heap.placement_address = VIRTUAL_ADDR_TO_PHYSICAL(&_end);
	if ((mmu.heap.placement_address % page_size) != 0) {
		mmu.heap.placement_address += (0x1000 - (mmu.heap.placement_address % page_size));
	}
	mmu.heap.page_count = 0x100;
	install_heap((void*)PHYSICAL_ADDR_TO_VIRTUAL(mmu.heap.placement_address), (void*)mmu.heap.placement_address, 0x100 * 0x1000);
}
*/

