/**
 * paging.c - Paging entry & memory management
 * System sources under license MIT
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/panic.h>
#include <kernel/stdio.h>

#include <arch/ld.h>
#include <cpu/cr.h>

#include "memory/physical.h"
#include "memory/virtual.h"

static uintptr_t S_PhysicalPageDirectoryAddr;
static uintptr_t S_VirtualPageDirectoryAddr;
static uintptr_t S_PhysicalPteDatabaseAddr;
static uintptr_t S_VirtualPteDatabaseAddr;

void Paging_set_page_directory(uintptr_t aVirtualAddr,
                               uintptr_t aPhysicalAddr) {
	S_VirtualPageDirectoryAddr  = aVirtualAddr;
	S_PhysicalPageDirectoryAddr = aPhysicalAddr;
	Cpu_write_cr3(S_PhysicalPageDirectoryAddr);
	Cpu_flush_tlb();
}

uintptr_t Paging_get_page_directory() { return S_VirtualPageDirectoryAddr; }

void Paging_set_pte_database(uintptr_t aVirtualAddr, uintptr_t aPhysicalAddr) {
	S_VirtualPteDatabaseAddr  = aVirtualAddr;
	S_PhysicalPteDatabaseAddr = aPhysicalAddr;
}

uint32_t* Paging_request_from_pte_database(uintptr_t aVirtualAddr) {
	uint32_t  pteDatabaseOffset = aVirtualAddr >> 22;
	uint32_t* pteDatabase       = (uint32_t*)Paging_get_pte_database();
	uint32_t  pageTableEntry    = pteDatabase[pteDatabaseOffset];
	if (pageTableEntry & 0x1) {
		return (uint32_t*)(((uint32_t)pteDatabase) + pteDatabaseOffset);
	}
	return NULL;
}

uintptr_t Paging_get_pte_database() { return S_VirtualPteDatabaseAddr; }

void Paging_add_pte(uintptr_t aTargetVirtualAddr, uintptr_t aPhysicalAddr) {
	unsigned offset = aTargetVirtualAddr >> 22;
	// uint32_t* pg_dir = (uint32_t*)PHYSICAL_ADDR_TO_VIRTUAL(Cpu_read_cr3());
	uint32_t* pg_dir    = (uint32_t*)Paging_get_page_directory();
	uint32_t* pte_pages = (uint32_t*)Paging_get_pte_database();
	pte_pages[offset]   = aPhysicalAddr | 0x3;
	Cpu_flush_tlb();
	uint32_t* pte = pte_pages + offset;
	memset(pte, 0x1000, 0);
	pg_dir[offset] = aPhysicalAddr | 0x3;
	Cpu_flush_tlb();
}

uint32_t Paging_map(uintptr_t aPhysicalAddr,
                    uintptr_t aVirtualAddr,
                    uint32_t  somePageFlags,
                    bool      anOverrideFlag) {

	if (aVirtualAddr % 0x1000 || aPhysicalAddr % 0x1000) {
		return EXIT_FAILURE;
	}
	uint32_t* pageTableEntry = Paging_request_from_pte_database(aVirtualAddr);
	if (!pageTableEntry) { // This region does not exist in the pages directory
		uint32_t  pteDatabaseOffset = aVirtualAddr >> 22;
		uintptr_t physicalPage      = Physical_memory_get_page(mt_AVAILABLE);
		if (!physicalPage) {
			PANIC("PAGING: could not create a new page table entry\n");
			return EXIT_FAILURE;
		}
		Paging_add_pte(pteDatabaseOffset * 0x400000, physicalPage);
		pageTableEntry = Paging_request_from_pte_database(aVirtualAddr);
		if (!pageTableEntry) {
			PANIC("PAGING: Failed to update pte database for %#x", aVirtualAddr);
		}
	}
	uint32_t pteOffset = aVirtualAddr >> 12 & 0x03FF;
	uint32_t* pageEntry = &pageTableEntry[pteOffset];
	if (*pageEntry & 0x1 && !anOverrideFlag) {
		printk("ERROR ::: PAGING: A pte is already present for this virtual "
		       "address\n");
		return EXIT_FAILURE;
	}
	*pageEntry = aPhysicalAddr | somePageFlags;
	Cpu_flush_tlb();
	return EXIT_SUCCESS;

	/*
	uint32_t* pg_dir = (uint32_t*)Paging_get_page_directory();
	if (aVirtualAddr % 0x1000 || aPhysicalAddr % 0x1000) {
	  return EXIT_FAILURE;
	}
	unsigned pg_dir_offset          = aVirtualAddr >> 22;
	uint32_t physical_pg_entry_addr = pg_dir[pg_dir_offset];

	if (!(physical_pg_entry_addr & 0x1)) {
	  uintptr_t page = Physical_memory_get_page(mt_AVAILABLE);
	  if (!page) {
	    printk("ERROR ::: PAGING: Asked page is not present, could not add a new "
	           "one\n");
	    return EXIT_FAILURE;
	  }
	  Paging_add_pte(pg_dir_offset * 0x400000, page);
	}
	physical_pg_entry_addr &= 0xFFFFF000;
	uint32_t* pg_table_entry =
	    PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)physical_pg_entry_addr);
	uint32_t  pte_offset = aVirtualAddr >> 12 & 0x03FF;
	uint32_t* pte_ptr    = &pg_table_entry[pte_offset];
	if (*pte_ptr & 0x1 && !anOverrideFlag) {
	  printk("ERROR ::: PAGING: A pte is already present for this virtual "
	         "address\n");
	  return EXIT_FAILURE;
	}
	*pte_ptr = aPhysicalAddr | somePageFlags;
	Cpu_flush_tlb();
	return EXIT_SUCCESS;
	 */
}

uintptr_t Paging_find_physical_address(uintptr_t aVirtualAddress) {
	uintptr_t physicalAddr = 0;
	if (aVirtualAddress % 0x1000) {
		return physicalAddr;
	}
	// unsigned pageDirOffset = aVirtualAddress >> 22;
	return 0;
}
