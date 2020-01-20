/**
 * map.c - map a physical address to a virtual addr
 * System sources under MIT license
 */

#include <stdint.h>
#include <stdlib.h>

#include <cpu/cr.h>
#include <cpu/mmu.h>
#include <arch/memlayout.h>
#include <kernel/stdio.h>

uint32_t		paging_map_physical(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags) {
	uint32_t	*pg_dir = (uint32_t*)PHYSICAL_ADDR_TO_VIRTUAL(read_cr3());

	if (virtual_addr % 0x1000 || physical_addr % 0x1000) {
		return EXIT_FAILURE;
	}
	unsigned pg_dir_offset = virtual_addr >> 22;
	printk("offset: %d\n", pg_dir_offset);
	uint32_t physical_pg_entry_addr = pg_dir[pg_dir_offset];
	if (!(physical_pg_entry_addr & 0x1)) {
		printk("ERROR ::: PAGING: Asked page is not present, could not add a new one\n");
		return EXIT_FAILURE;
	}
	physical_pg_entry_addr &= 0xFFFFD000;
	uint32_t *pg_table_entry = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)physical_pg_entry_addr);
	uint32_t pte_offset = virtual_addr >> 12 & 0x03FF;
	printk("pte offset: %d\n", pte_offset);
	uint32_t *pte_ptr = &pg_table_entry[pte_offset];
	printk("physical pte addr: %#x\n", &pte_ptr);
	*pte_ptr = physical_addr | flags;
	printk("pte entry is: %#x\n", *pte_ptr);
	flush_tlb();
	return EXIT_SUCCESS;
}

