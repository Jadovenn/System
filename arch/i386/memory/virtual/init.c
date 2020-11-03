/**
 * mmu_init.c - Memory Management Unit Installation
 * System sources under license MIT
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hal/memory.h>
#include <kernel/panic.h>
#include <kernel/stdio.h>

#include <arch/ld.h>
#include <arch/paging.h>
#include <cpu/cr.h>

#include "memory/physical.h"

/**
 * Virtual memory init sequence (paging):
 *	1. allocate & init physical page for page_table_entries
 *	2. allocate & init physical page for page_boot_directory
 *	3. flush tlb
 *	4. set correct r/w access for text section
 *	5. set correct r/w access for rodata section
 *	6. init heap, recycle bootG_Page_directory as pte for 0xD0000000
 */

/********************************
 **    Globals definition      **
 ********************************/

uint32_t G_Page_directory;
uint32_t G_Page_table_entries;

/********************************
 **   Init static function     **
 ********************************/

static __inline void S_set_section_text_ro() {
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&G_End_code);
	uint32_t vaddr         = (uint32_t)&G_Start_kernel;
	uint32_t paddr         = VIRTUAL_ADDR_TO_PHYSICAL(&G_Start_kernel);
	unsigned page_mapped   = 0;
	for (; paddr < text_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		pg_map(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

static __inline void S_set_section_rodata_ro() {
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&G_End_rodata);
	uint32_t vaddr           = (uint32_t)&G_Start_rodata;
	uint32_t paddr           = VIRTUAL_ADDR_TO_PHYSICAL(&G_End_rodata);
	unsigned page_mapped     = 0;
	for (; paddr < rodata_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		pg_map(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

static void S_heap_init() {
	pg_add_pte(0xD0000000,
	           VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&boot_page_directory));
	uintptr_t page = Physical_memory_get_page(mt_AVAILABLE);
	if (!page) {
		PANIC("Not enought memory. Could not init virtual memory.");
	}
	pg_map(page, 0xD0000000, 0x003, true);
	Hal_init_memory_allocator(0xD0000000, 0x1000);
}

static void S_vmap_page_directory() {
	// alloc page contiguously
	G_Page_directory = G_Page_table_entries + 0x1000;
	uint32_t* vstart = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)G_Page_directory);
	uint32_t* pg_dir = PHYSICAL_PTR_TO_VIRTUAL(Cpu_read_cr3());
	Physical_memory_set_page(G_Page_directory, pms_PRESENT);
	pg_map(G_Page_directory, (uint32_t)vstart, 0x003, false);
	// init by copy of the boot page directory and remove itself from the boot
	// table
	memcpy(vstart, pg_dir, 0x1000);
}

static void S_vmap_page_tables_entries() {
	// alloc page contiguously
	G_Page_table_entries = G_Physical_mmap_end + (0x1000 - G_Physical_mmap_end);
	uint32_t* vstart = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)G_Page_table_entries);
	Physical_memory_set_page(G_Page_table_entries, pms_PRESENT);
	pg_map(G_Page_table_entries, (uint32_t)vstart, 0x003, false);
	// init by adding manually the boot page directory
	memset(vstart, 0x1000, 0x0);
	unsigned offset = PHYSICAL_ADDR_TO_VIRTUAL((uint32_t)&boot_page_table) >> 22;
	vstart[offset]  = (uint32_t)&boot_page_table | 0x003;
}

/********************************
 **      Public function       **
 ********************************/

void Init_virtual_memory() {
	S_vmap_page_tables_entries();
	S_vmap_page_directory();
	Cpu_write_cr3(G_Page_directory);
	Cpu_flush_tlb();
	S_set_section_text_ro();
	S_set_section_rodata_ro();
	S_heap_init();
}
