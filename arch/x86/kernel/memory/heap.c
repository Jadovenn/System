/**
 * heap.c - Heap allocation for kernel space
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/types.h>

extern uint32_t	end; // defined in the linker script
uint32_t	placement_address = (uint32_t)&end;

static uint32_t	__kmalloc_internal(uint32_t size, bool align, uint32_t *physical_addr) {
	if (align == TRUE && (placement_address & 0xfffff000)) {
		placement_address &= 0xfffff000;
		placement_address += 0x1000;
	}
	if (physical_addr) {
		*physical_addr = placement_address;
	}
	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}

uint32_t	kmalloc(uint32_t size) {
	return __kmalloc_internal(size, FALSE, NULL);
}

uint32_t	kmalloc_aligned(uint32_t size) {
	return __kmalloc_internal(size, TRUE, NULL);
}

uint32_t	kmalloc_physical(uint32_t size, uint32_t *phys) {
	return __kmalloc_internal(size, FALSE, phys);
}

uint32_t	kmalloc_physical_align(uint32_t size, uint32_t *phys) {
	return __kmalloc_internal(size, TRUE, phys);
}

