/**
 * map.c - map a physical address to a virtual addr
 * System sources under MIT license
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <cpu/cr.h>
#include <cpu/mmu.h>
#include <arch/memlayout.h>
#include <kernel/stdio.h>

uint32_t		paging_map_physical(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags, bool override) {
	uint32_t	*pg_dir = (uint32_t*)PHYSICAL_ADDR_TO_VIRTUAL(read_cr3());

	if (virtual_addr % 0x1000 || physical_addr % 0x1000) {
		return EXIT_FAILURE;
	}
	unsigned pg_dir_offset = virtual_addr >> 22;
	uint32_t physical_pg_entry_addr = pg_dir[pg_dir_offset];
	if (!(physical_pg_entry_addr & 0x1)) {
		printk("ERROR ::: PAGING: Asked page is not present, could not add a new one\n");
		return EXIT_FAILURE;
	}
	physical_pg_entry_addr &= 0xFFFFD000;
	uint32_t *pg_table_entry = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)physical_pg_entry_addr);
	uint32_t pte_offset = virtual_addr >> 12 & 0x03FF;
	uint32_t *pte_ptr = &pg_table_entry[pte_offset];
	if (*pte_ptr & 0x1 && !override) {
		printk("ERROR ::: PAGING: A pte is already present for this virtual address\n");
		return EXIT_FAILURE;
	}
	*pte_ptr = physical_addr | flags;
	flush_tlb();
	return EXIT_SUCCESS;
}

