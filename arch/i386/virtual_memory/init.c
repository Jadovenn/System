/**
 * mmu_init.c - Memory Management Unit Installation
 * System sources under license MIT
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <kernel/panic.h>
#include <kernel/stdio.h>

#include <arch/memlayout.h>
#include <arch/paging.h>
#include <arch/physical_memory.h>

#include <cpu/cr.h>
#include <multiboot.h>

/**
 * Paging init sequence:
 *	1. allocate & init physical page for page_table_entries
 *	2. allocate & init physical page for page_boot_directory
 *	3. flush tlb
 *	4. set correct r/w access for text section
 *	5. set correct r/w access for rodata section
 *	6. init heap, recycle boot_page_directory as pte for 0xD0000000
 */

// GLOBAL definitions
uint32_t _page_directory;
uint32_t _page_table_entries;

static __inline void __set_section_text_ro() {
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_code);
	uint32_t vaddr         = (uint32_t)&_kernel_start;
	uint32_t paddr         = VIRTUAL_ADDR_TO_PHYSICAL(&_kernel_start);
	unsigned page_mapped   = 0;
	for (; paddr < text_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		pg_map(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

static __inline void __set_section_rodata_ro() {
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_rodata);
	uint32_t vaddr           = (uint32_t)&_rodata;
	uint32_t paddr           = VIRTUAL_ADDR_TO_PHYSICAL(&_rodata);
	unsigned page_mapped     = 0;
	for (; paddr < rodata_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		pg_map(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

static void* libc_get_page_callback(size_t* size, size_t request) {
	*size = 0;
	(void)request;
	return NULL;
}

void heap_init() {
	pg_add_pte(0xD0000000,
	           VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&boot_page_directory));
	uintptr_t page = pmm_get_page(MEMORY_AVAILABLE);
	if (!page) {
		PANIC("Not enought memory. Could not init virtual memory.");
	}
	pg_map(page, 0xD0000000, 0x003, true);
	libc_init_allocator(0xD0000000, 0x1000, &libc_get_page_callback);
}

static void _vmap_page_directory() {
	// alloc page contiguously
	_page_directory  = _page_table_entries + 0x1000;
	uint32_t* vstart = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)_page_directory);
	uint32_t* pg_dir = PHYSICAL_PTR_TO_VIRTUAL(read_cr3());
	pmm_set_page(_page_directory, PM_PAGE_PRESENT);
	pg_map(_page_directory, (uint32_t)vstart, 0x003, false);
	// init by copy of the boot page directory and remove itself from the boot
	// table
	memcpy(vstart, pg_dir, 0x1000);
}

static void _vmap_page_tables_entries() {
	// alloc page contiguously
	_page_table_entries = _physical_mmap_end + (0x1000 - _physical_mmap_end);
	uint32_t* vstart    = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)_page_table_entries);
	pmm_set_page(_page_table_entries, PM_PAGE_PRESENT);
	pg_map(_page_table_entries, (uint32_t)vstart, 0x003, false);
	// init by adding manually the boot page directory
	memset(vstart, 0x1000, 0x0);
	unsigned offset = PHYSICAL_ADDR_TO_VIRTUAL((uint32_t)&boot_page_table) >> 22;
	vstart[offset]  = (uint32_t)&boot_page_table | 0x003;
}

void paging_init(multiboot_info* header) {
	(void)header;
	_vmap_page_tables_entries();
	_vmap_page_directory();
	write_cr3(_page_directory);
	flush_tlb();
	__set_section_text_ro();
	__set_section_rodata_ro();
	heap_init();
}
