/**
 * memory_init.c - Memory init
 * System sources under license MIT
 */

#include <arch/ld.h>

#include "memory/physical.h"
#include "memory/virtual.h"

void Init_memory(uintptr_t aMultibootMmapAddr, uint32_t aMultibootMmapLength) {
	Init_physical_memory(aMultibootMmapAddr, aMultibootMmapLength);
	Init_virtual_memory();
}
