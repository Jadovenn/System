/**
 * memory.h - Kernel Memory Management API
 * System sources under license MIT
 */

#include <stdint.h>

#include <hal/memory.h>
#include <kernel/panic.h>

#include "memory/physical.h"
#include "memory/virtual.h"

static uintptr_t S_HeapBreak  = 0x00000000;
static uintptr_t S_HeapCursor = 0x00000000;

/********************************
 ** Heap private function      **
 ********************************/

/********************************
 ** Heap public init function  **
 ********************************/

void Init_heap(uintptr_t anHeapBreak, size_t aSize) {
	if (aSize % 0x1000) {
		PANIC("Size provided for the heap must be page_size aligned.");
	}
	S_HeapBreak  = anHeapBreak;
	S_HeapCursor = anHeapBreak + aSize;
}

/********************************
 ** Hal public functions       **
 ********************************/

size_t Hal_get_page_size() { return 0x1000; }

uintptr_t Hal_increase_heap_break(size_t aSize) {
	size_t pageAlignedRequest = aSize;
	pageAlignedRequest += 0x1000 - (aSize % 0x1000);
	for (size_t allocated = 0; allocated < pageAlignedRequest;
	     allocated += 0x1000) {
		uintptr_t physicalPage = Physical_memory_get_page(mt_AVAILABLE);
		if (!physicalPage) {
			return S_HeapCursor;
		}
		uint32_t result = Paging_map(physicalPage, S_HeapCursor, 0x3, false);
		if (result == EXIT_FAILURE) {
			return S_HeapCursor;
		}
		S_HeapCursor += 0x1000;
	}
	return S_HeapCursor;
}

uintptr_t Hal_set_break(uintptr_t aBreakAddr) { (void)aBreakAddr; return 0; }
