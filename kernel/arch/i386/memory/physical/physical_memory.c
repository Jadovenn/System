/**
 * physical_memory.c - Physical Memory Management
 * System sources under license MIT
 */

#include <bitset.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "memory/physical.h"

Physical_memory_region_t* G_Physical_memory_map = NULL;

uint32_t Physical_memory_set_page(const uintptr_t aPhysicalAddr, Physical_memory_state_e aState)
{
	Physical_memory_region_t* region = NULL;
	if (aPhysicalAddr % 0x1000) {
		return EXIT_FAILURE;
	}
	{
		Physical_memory_region_t* idx = G_Physical_memory_map;
		while (idx) {
			if (idx->startAddr <= aPhysicalAddr && aPhysicalAddr < idx->endAddr) {
				region = idx;
				break;
			}
			idx = idx->next;
		}
		if (!region || region->bitset == NULL) {
			return EXIT_FAILURE;
		}
	}
	uint32_t offset = ((aPhysicalAddr - region->startAddr) / 0x1000) / 32;
	uint32_t bit    = ((aPhysicalAddr - region->startAddr) / 0x1000) % 32;
	bit             = 31 - bit;
	if (aState == pms_PRESENT) {
		_set_bit(region->bitset[offset], bit);
	} else {
		_unset_bit(region->bitset[offset], bit);
	}
	return EXIT_SUCCESS;
}

uint32_t Physical_memory_set_region(const uintptr_t         aPhysicalStartAddr,
                                    const uintptr_t         aPhysicalEndAddr,
                                    Physical_memory_state_e aState)
{
	uint32_t offset = aPhysicalStartAddr;
	if (aPhysicalStartAddr > aPhysicalEndAddr) {
		return EXIT_FAILURE;
	}
	while (offset <= aPhysicalEndAddr) {
		Physical_memory_set_page(offset, aState);
		offset += 0x1000;
	}
	return EXIT_SUCCESS;
}

void Physical_memory_release_page(uintptr_t aPhysicalAddress)
{
	Physical_memory_set_page((uint32_t)aPhysicalAddress, pms_NOT_PRESENT);
}

static uintptr_t
_compute_addr_and_alloc(Physical_memory_region_t* aRegion, uint32_t anOffset, uint32_t aBitSet, uint32_t aBitCount)
{
	uintptr_t addr = aRegion->startAddr + (anOffset * 32 * 0x1000) + (aBitCount * 0x1000);
	aRegion->bitset[anOffset] |= aBitSet;
	return addr;
}

uintptr_t Physical_memory_get_page(Memory_type_e aMemoryType)
{
	for (Physical_memory_region_t* region = G_Physical_memory_map; region; region = region->next) {
		unsigned idx = 0;
		if (region->type != aMemoryType || region->bitset == NULL) {
			continue;
		}
		while (idx < region->page_nb / 32) { // search in bitset
			if (region->bitset[idx] != 0xFFFFFFFF) {
				uint32_t bit       = 0x80000000;
				uint32_t bit_count = 0;
				while (bit) {                         // search in dword
					if (!(region->bitset[idx] & bit)) { // free page found
						return _compute_addr_and_alloc(region, idx, bit, bit_count);
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
