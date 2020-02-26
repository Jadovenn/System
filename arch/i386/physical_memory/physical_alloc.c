/**
 * physical_memory.c - Physical Memory Management
 * System sources under license MIT
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <bitset.h>

#include <api/mm.h>
#include <arch/physical_memory.h>

pmm_region_t	*physical_memory_map = NULL;

uint32_t	pmm_set_page(const uintptr_t p_addr, bool value) {
	pmm_region_t *region = NULL;
	if (p_addr % 0x1000) {
		return EXIT_FAILURE;
	}
	{
		pmm_region_t *idx = physical_memory_map;
		while (idx) {
			if (idx->pstart <= p_addr && p_addr < idx->pend) {
				region = idx;
				break;
			}
			idx = idx->next;
		}
		if (!region || region->bitset == NULL) { // if addr not accessable
			return EXIT_FAILURE;
		}
	}
	uint32_t offset = ((p_addr - region->pstart) / 0x1000) / 32;
	uint32_t bit = ((p_addr - region->pstart) / 0x1000) % 32;
	bit = 31 - bit;
	if (value) {
		_set_bit(region->bitset[offset], bit);
	}
	else {
		_unset_bit(region->bitset[offset], bit);
	}
	return EXIT_SUCCESS;
}

uint32_t	pmm_set_region(const uintptr_t p_start_addr, const uintptr_t p_end_addr, bool value) {
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

void	pmm_release_page(uintptr_t addr) {
	pmm_set_page((uint32_t)addr, false);
}

uintptr_t	__compute_addr_and_alloc(pmm_region_t *region, uint32_t offset, uint32_t bit, uint32_t bit_count) {
	uintptr_t addr = region->pstart + (offset * 32 * 0x1000) + (bit_count * 0x1000);
	region->bitset[offset] |= bit;
	return addr;
}

uintptr_t	pmm_get_page(uint8_t type) {
	for (pmm_region_t *region = physical_memory_map;
			region; region = region->next) {
		unsigned idx = 0;
		if (region->type != type || region->bitset == NULL) {
			continue;
		}
		while (idx < region->page_nb / 32) { // search in bitset
			if (region->bitset[idx] != 0xFFFFFFFF) {
				uint32_t bit = 0x80000000;
				uint32_t bit_count = 0;
				while (bit) { // search in dword
					if (!(region->bitset[idx] & bit)) { // free page found
						return __compute_addr_and_alloc(region, idx, bit, bit_count);
					}
					bit = bit >> 1;
					bit_count += 1;
				}
			}
			idx += 1;
		}
	}
	return 0x0;
}

