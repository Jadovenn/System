/**
 * mmap.c - Memory map
 * System sources under license MIT
 */

#include <stdlib.h>

#include <hal/memory.h>

#include "memory/physical.h"
#include "memory/virtual.h"

/********************************
 ** Map private function       **
 ********************************/

static void* _find_virtual_addr(Hal_memory_mode_e aMode, size_t aPageCount) {
	Virtual_memory_type_e memoryType = vmt_DATA;
	if (aMode & map_EXECUTE) {
		memoryType = vmt_USERSPACE;
	}
	Virtual_memory_area_t* area = G_Kernel_vma;
	while (area->type != vmt_END) {
		if (area->type == memoryType) {
			break;
		}
		area++;
	}
	if (area->type == vmt_END) {
		return MAP_FAILED;
	}
	uintptr_t vstart = area->start;
	uintptr_t vend   = vstart;
	// size_t    vstartPteOffset = vstart >> 12 & 0x03FF;
	uint32_t* pte = Paging_request_from_pte_database(vstart);
	while ((vend - vstart) / 0x1000 < aPageCount) {
		size_t offset = vend >> 12 & 0x03FF;

		// Current pte is empty, increase 4Mib
		if (pte == NULL) {
			vend += 0x400000;
			pte = Paging_request_from_pte_database(vend);
			continue;
		}

		// End of current pte go to the next one
		if (offset == 0xFFF) {
			pte = Paging_request_from_pte_database(vend);
		}

		// Current pte is used jump to the next free section
		if (pte[offset] != 0) {
			while (pte[offset] != 0) {
				if (offset == 0xFFF) {
					pte = Paging_request_from_pte_database(vend);
					if (!pte) {
						break;
					}
				}
				vend += 0x1000;
				offset = vend >> 12 & 0x03FF;
			}
			vstart = vend;
			if (vstart > area->end) {
				return MAP_FAILED;
			}
			continue;
		}

		// End of current pte go to the next one
		if (offset == 0xFFF) {
			pte = Paging_request_from_pte_database(vend);
		}
		vend += 0x1000;
	}
	return (void*)vstart;
} // O(N)

static uint32_t _compute_page_entry_flags(Hal_memory_mode_e aMode) {
	uint32_t flags = 1;
	if (aMode & map_READWRITE) {
		flags |= 0x2;
	}
	return flags;
}

/****************************************
 ** Hal_ mapping public implementation **
 ****************************************/

void* Hal_mmap(uintptr_t         aPhysicalAddr,
               size_t            aPageCount,
               Hal_memory_mode_e aMode) {
	if (aMode & map_ANONYMOUS && aMode & map_POPULATE) {
		return MAP_FAILED;
	}
	if (aPhysicalAddr != 0 && aMode & map_POPULATE) {
		return MAP_FAILED;
	}
	if (aMode & map_READWRITE && aMode & map_READONLY) {
		return MAP_FAILED;
	}
	if (aMode & map_EXECUTE && aMode & map_DATA) {
		return MAP_FAILED;
	}
	if (aPhysicalAddr % 0x1000 || aPageCount == 0) {
		return MAP_FAILED;
	}
	void* vstart = _find_virtual_addr(aMode, aPageCount);
	if (vstart == MAP_FAILED) {
		return MAP_FAILED;
	}
	uint32_t flags = _compute_page_entry_flags(aMode);

	for (size_t mapped = 0; mapped < aPageCount; mapped++) {
		uintptr_t physicalAddr;
		if (aMode & map_ANONYMOUS) {
			physicalAddr = aPhysicalAddr + 0x1000 * mapped;
		} else if (aMode & map_POPULATE) {
			physicalAddr = Physical_memory_get_page(mt_AVAILABLE);
		} else {
			Hal_munmap(vstart, mapped);
			return MAP_FAILED;
		}
		Paging_add_page_entry((uintptr_t)vstart + 0x1000 * mapped, physicalAddr,
		                      flags);
	}
	return (void*)vstart;
} // O(N) - linear in total virtual memory area size

int Hal_munmap(void* aVirtualAddr, size_t aPageCount) {
	if ((uintptr_t)aVirtualAddr % 0x1000) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < aPageCount; i++) {
		Paging_clear_page_entry((uintptr_t)aVirtualAddr + 0x1000 * i);
	}
	return EXIT_SUCCESS;
} // O(1) - constant for address search

void* Hal_get_associated_addr(uintptr_t aVirtualAddr) {
	uintptr_t virtualAddr    = aVirtualAddr % 0x1000;
	uint32_t* pageTableEntry = Paging_request_from_pte_database(virtualAddr);
	if (!pageTableEntry) {
		return MAP_FAILED;
	}
	uint32_t  pteOffset = virtualAddr >> 12 & 0x03FF;
	uint32_t* pageEntry = &pageTableEntry[pteOffset];
	if ((*pageEntry & 0x1) == 0) {
		return MAP_FAILED;
	}
	return (void*)((*pageEntry & 0xFFFFF000) + aVirtualAddr % 0x1000);
} // O(1)
