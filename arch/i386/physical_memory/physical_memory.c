/**
 * physical_memory.c - Physical Memory Management
 * System sources under license MIT
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <bitset.h>

#include "physical_memory.h"

pmm_region_t	*physical_memory_map = NULL;

uint32_t	pmm_set_page(const uint32_t p_addr, bool value) {
	pmm_region_t *region = NULL;
	if (p_addr % 0x1000) { // check alignement
		return EXIT_FAILURE;
	}
	{ // find coresponding region
		pmm_region_t *idx = physical_memory_map;
		while (idx) {
			if (p_addr - idx->physical_addr < idx->page_nb * 0x1000) {
				region = idx;
				break;
			}
			idx = idx->next;
		}
		if (!region) { // if addr not accessable
			return EXIT_FAILURE;
		}
	}
	uint32_t offset = ((p_addr - region->physical_addr) / 0x1000) / 32;
	uint32_t bit = ((p_addr - region->physical_addr) / 0x1000) % 32;
	bit = 31 - bit;
	if (value) {
		_set_bit(region->bitset[offset], bit);
	}
	else {
		_unset_bit(region->bitset[offset], bit);
	}
	return EXIT_SUCCESS;
}

uint32_t	pmm_set_region(const uint32_t p_start_addr, const uint32_t p_end_addr, bool value) {
	uint32_t offset = p_start_addr;
	if (p_start_addr > p_end_addr) {
		return EXIT_FAILURE;
	}
	while (offset <= p_end_addr) {
		pmm_set_page(offset, value);
		offset += 0x1000;
	}
	return EXIT_SUCCESS;
}

void	p_free(void *addr) {
	pmm_set_page((uint32_t)addr, false);
}

void	*p_alloc(void) {
	pmm_region_t *region = physical_memory_map;
	return NULL;
}

