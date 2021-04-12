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

#include "memory/boot/boot_memory.h"
#include "memory/physical.h"
#include "memory/virtual.h"

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

static __inline void _set_section_text_ro()
{
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&G_End_code);
	uint32_t vaddr         = (uint32_t)&G_Start_kernel;
	uint32_t paddr         = VIRTUAL_ADDR_TO_PHYSICAL(&G_Start_kernel);
	unsigned page_mapped   = 0;
	for (; paddr < text_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		Paging_update_page_entry_flags(vaddr, 0x1);
		page_mapped += 1;
	}
}

static __inline void _set_section_rodata_ro()
{
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&G_End_rodata);
	uint32_t vaddr           = (uint32_t)&G_Start_rodata;
	uint32_t paddr           = VIRTUAL_ADDR_TO_PHYSICAL(&G_End_rodata);
	unsigned page_mapped     = 0;
	for (; paddr < rodata_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		Paging_update_page_entry_flags(vaddr, 0x1);
		page_mapped += 1;
	}
}

static void _heap_init()
{
	uintptr_t heapBreak = 0x00000000;
	for (Virtual_memory_area_t* area = G_Kernel_vma; area; area++) {
		if (area->type == vmt_HEAP) {
			heapBreak = area->start;
			break;
		}
	}
	Paging_add_pte(heapBreak, VIRTUAL_ADDR_TO_PHYSICAL((uint32_t)&G_Boot_page_directory));
	uintptr_t page = Physical_memory_get_page(mt_AVAILABLE);
	if (!page) {
		PANIC("Not enough memory. Could not init virtual memory.");
	}
	Paging_clear_page_entry(heapBreak);
	Paging_add_page_entry(page, heapBreak, 0x003);
	Init_heap(heapBreak, 0x1000);
}

static void _vmap_page_directory()
{
	// alloc page contiguously
	G_Page_directory = G_Page_table_entries + 0x1000;
	uint32_t* vstart = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)G_Page_directory);
	uint32_t* pg_dir = PHYSICAL_PTR_TO_VIRTUAL(Cpu_read_cr3());
	Physical_memory_set_page(G_Page_directory, pms_PRESENT);
	Boot_paging_map(G_Page_directory, (uint32_t)vstart, 0x003, false);
	// init by copy of the boot page directory and remove itself from the boot
	// table
	memcpy(vstart, pg_dir, 0x1000);
}

static uintptr_t _vmap_page_tables_entries()
{
	// alloc page contiguously
	G_Page_table_entries = G_Physical_mmap_end + (0x1000 - G_Physical_mmap_end % 0x1000);
	uint32_t* vstart     = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)G_Page_table_entries);
	Physical_memory_set_page(G_Page_table_entries, pms_PRESENT);
	Boot_paging_map(G_Page_table_entries, (uint32_t)vstart, 0x003, false);
	// init by adding manually the boot page directory
	memset(vstart, 0x0, 0x1000);
	size_t    bootPteOffset       = ((uintptr_t)&G_Boot_page_table) >> 22;
	uintptr_t physicalBootPteAddr = VIRTUAL_ADDR_TO_PHYSICAL(((uintptr_t)&G_Boot_page_table));
	vstart[bootPteOffset]         = physicalBootPteAddr | 0x3;
	vstart[0]                     = G_Page_table_entries | 0x3;

	uintptr_t dataAddr = 0x00000000;
	for (Virtual_memory_area_t* area = G_Kernel_vma; area; area++) {
		if (area->type == vmt_DATA) {
			dataAddr = area->start;
			break;
		}
	}

	Boot_paging_add_pte(dataAddr, G_Page_table_entries);
	vstart[dataAddr >> 22] = G_Page_table_entries | 0x3;
	return dataAddr;
}

/********************************
 **      Public function       **
 ********************************/

void Init_virtual_memory()
{
	uintptr_t pageTableEntriesDatabase = _vmap_page_tables_entries();
	_vmap_page_directory();
	Paging_set_page_directory(PHYSICAL_ADDR_TO_VIRTUAL((uint32_t)G_Page_directory), G_Page_directory);
	Paging_set_pte_database(PHYSICAL_ADDR_TO_VIRTUAL(G_Page_table_entries), G_Page_table_entries,
	                        pageTableEntriesDatabase);
	_set_section_text_ro();
	_set_section_rodata_ro();
	_heap_init();
}
