/**
 * memory.h - Kernel Memory Management API
 * System sources under license MIT
 */

#include <stdint.h>

#include <hal/memory.h>
#include <kernel/panic.h>

#include "memory/physical.h"
#include "memory/virtual.h"

static uintptr_t S_HeapStart  = 0x00000000;
static uintptr_t S_HeapBreak = 0x00000000;
static uintptr_t S_HeapEnd = 0x00000000;

/********************************
 ** Heap private function      **
 ********************************/

void _reduce_heap_size() {
	if (S_HeapEnd - S_HeapBreak >= 0x1000) {
		uintptr_t cursor = S_HeapBreak + (0x1000 - (S_HeapBreak % 0x1000));
		while (cursor != S_HeapEnd) {
			void* physicalAddr = Paging_find_physical_address(cursor);
			if (physicalAddr == (void*)-1) {
				return;
			}
			Paging_clear_page_entry(cursor);
			Physical_memory_release_page((uintptr_t)physicalAddr);
			cursor += 0x1000;
		}
	}
}

/********************************
 ** Heap public init function  **
 ********************************/

void Init_heap(uintptr_t anHeapBreak, size_t aSize) {
	if (aSize % 0x1000) {
		PANIC("Size provided for the heap must be page_size aligned.");
	}
	S_HeapStart = anHeapBreak;
	S_HeapBreak = anHeapBreak;
	S_HeapEnd = anHeapBreak + aSize;
}

/********************************
 ** Hal public functions       **
 ********************************/

size_t Hal_get_page_size() { return 0x1000; }

void *Hal_sbrk(size_t aSize) {
	if (aSize == 0) {
		return (void*)S_HeapBreak;
	}
	if (S_HeapEnd - S_HeapBreak > aSize) {
		void* heapBreak = (void*)S_HeapBreak;
		S_HeapBreak += aSize;
		return heapBreak;
	}
	size_t pageAlignedRequest = aSize;
	pageAlignedRequest += 0x1000 - (aSize % 0x1000);
	for (size_t allocated = 0; allocated < pageAlignedRequest;
			 allocated += 0x1000) {
		uintptr_t physicalPage = Physical_memory_get_page(mt_AVAILABLE);
		if (!physicalPage) {
			return (void*)-1;
		}
		uint32_t result = Paging_add_page_entry(physicalPage, S_HeapEnd, 0x3);
		if (result == EXIT_FAILURE) {
			return (void*)-1;
		}
		S_HeapEnd += 0x1000;
	}
	void* heapBreak = (void*)S_HeapBreak;
	S_HeapBreak += aSize;
	return heapBreak;
}

int Hal_brk(void* anAddress) {
	if ((uintptr_t)anAddress >= S_HeapBreak) {
		void* result = Hal_sbrk((uintptr_t)anAddress - S_HeapBreak);
		if (result == NULL) {
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	else {
		if ((uintptr_t)anAddress < S_HeapStart) {
			return EXIT_FAILURE;
		}
		S_HeapBreak = (uintptr_t)anAddress;
		_reduce_heap_size();
	}
	return EXIT_SUCCESS;
}
