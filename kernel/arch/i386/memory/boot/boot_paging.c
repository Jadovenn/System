/**
 * boot_memory.c - Boot time memory management
 * System sources under license MIT
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/panic.h>
#include <kernel/stdio.h>

#include <arch/ld.h>
#include <cpu/cr.h>

#include "memory/boot/boot_memory.h"
#include "memory/physical.h"
#include "memory/virtual.h"

void Boot_paging_add_pte(uintptr_t vaddr, uintptr_t paddr)
{
	if (vaddr % 0x400000) {
		PANIC("PAGING ::: boot add pte error %#x is not divisible by 4Mib\n");
	}
	unsigned  offset = vaddr >> 22;
	uint32_t* pg_dir = (uint32_t*)PHYSICAL_ADDR_TO_VIRTUAL(Cpu_read_cr3());
	/*
	uint32_t* pte_pages =
	    PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)G_Page_table_entries);
	pte_pages[offset] = paddr | 0x3;
	Cpu_flush_tlb();
	uint32_t* pte = pte_pages + offset;
	memset(pte, 0x0, 0x1000);
	*/
	pg_dir[offset]   = paddr | 0x3;
	Cpu_flush_tlb();
}

uint32_t Boot_paging_map(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags, bool override)
{
	uint32_t* pg_dir = (uint32_t*)PHYSICAL_ADDR_TO_VIRTUAL(Cpu_read_cr3());

	if (virtual_addr % 0x1000 || physical_addr % 0x1000) {
		return EXIT_FAILURE;
	}
	unsigned pg_dir_offset          = virtual_addr >> 22;
	uint32_t physical_pg_entry_addr = pg_dir[pg_dir_offset];
	if (!(physical_pg_entry_addr & 0x1)) {
		uintptr_t page = Physical_memory_get_page(mt_AVAILABLE);
		if (!page) {
			PANIC("ERROR ::: Boot paging algorithm does not support new page table "
			      "entry");
		}
		Boot_paging_add_pte(pg_dir_offset * 0x400000, page);
	}
	physical_pg_entry_addr &= 0xFFFFF000;
	uint32_t* pg_table_entry = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)physical_pg_entry_addr);
	uint32_t  pte_offset     = virtual_addr >> 12 & 0x03FF;
	uint32_t* pte_ptr        = &pg_table_entry[pte_offset];
	if (*pte_ptr & 0x1 && !override) {
		printf("ERROR ::: PAGING: A pte is already present for this virtual "
		       "address\n");
		return EXIT_FAILURE;
	}
	*pte_ptr = physical_addr | flags;
	Cpu_flush_tlb();
	return EXIT_SUCCESS;
}
