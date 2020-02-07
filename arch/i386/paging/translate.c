/**
 * translate.c - translate v_addr to p_addr
 * System sources under license MIT
 */

#include <stdint.h>
#include <cpu/cr.h>
#include <arch/memlayout.h>

uint32_t	paging_translate_virtual(uint32_t virtual_addr) {
	uint32_t	*pg_dir = (uint32_t*)PHYSICAL_ADDR_TO_VIRTUAL(read_cr3());

	uint32_t aligned_addr = virtual_addr - (virtual_addr % 0x1000);
	uint32_t pg_dir_offset = aligned_addr >> 22;
	uint32_t physical_pg_entry_addr = pg_dir[pg_dir_offset];
	if (!(physical_pg_entry_addr & 0x1)) {
		return 0;
	}
	physical_pg_entry_addr &= 0xFFFFF000;
	uint32_t *pg_table_entry = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)physical_pg_entry_addr);
	uint32_t pte_offset = virtual_addr >> 12 & 0x03FF;
	uint32_t *pte_ptr = &pg_table_entry[pte_offset];
	if (!(*pte_ptr & 0x001)) {
		return 0;
	}
	return ((uint32_t)*pte_ptr & 0xFFFFF000) + (virtual_addr & 0x00000FFF);
}

