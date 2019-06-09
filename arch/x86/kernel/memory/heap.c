/**
 * heap allocation for kernel space
 */

#include <kernel/types.h>

uint32_t	placement_address = 0x90000;

uint32_t	kmalloc(uint32_t size) {
	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}

uint32_t	kmalloc_aligned(uint32_t size) {
	placement_address &= 0xFFFFF000;
	placement_address += 0x1000;
	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}

uint32_t	kmalloc_physical(uint32_t size, uint32_t *phys) {
	if (phys) {
		*phys = placement_address;
	}
	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}

uint32_t	kmalloc_physical_align(uint32_t size, uint32_t *phys) {
	placement_address &= 0xFFFFF000;
	placement_address += 0x1000;
	if (phys) {
		*phys = placement_address;
	}
	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}

