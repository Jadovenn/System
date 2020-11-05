/**
 * virtual.h - Virtual Memory Management
 * System sources under license MIT
 */

#ifndef I386_VIRTUAL_H
#define I386_VIRTUAL_H

#include <stdbool.h>
#include <stdint.h>

#include <hal/memory.h>

/********************************
 **     Structure and Enum     **
 ********************************/

typedef enum Virtual_memory_type {
	vmt_USERSPACE    = 0x00000001,
	vmt_BIOS         = 0x00000010,
	vmt_RESERVED     = 0x00000100,
	vmt_KERNEL_IMAGE = 0x00001000,
	vmt_LOW_MEMORY   = 0x00010000,
	vmt_HEAP         = 0x00100000,
	vmt_DATA         = 0x01000000,
	vmt_DAEMON       = 0x10000000,
	vmt_END
} Virtual_memory_type_e;

typedef struct Virtual_memory_area {
	uintptr_t             start;
	uintptr_t             end;
	Virtual_memory_type_e type;
} Virtual_memory_area_t;

/********************************
 **    Globals declaration     **
 ********************************/

extern uint32_t              G_Page_directory;
extern uint32_t              G_Page_table_entries;
extern Virtual_memory_area_t G_Kernel_vma[];

/********************************
 **     Public functions       **
 ********************************/

/**
 * Initialise the heap
 * @param anHeapBreak - the heap break
 * @param aSize - a size if some page are already mapped or 0
 */
void Init_heap(uintptr_t anHeapBreak, size_t aSize);

/**
 * Initialize the virtual memory, physical memory must be initialized first
 */
void Init_virtual_memory();

/**
 * Initialize memory
 * @param aMultibootMmapAddr
 * @param aMultibootMmapLength
 */
void Init_memory(uintptr_t aMultibootMmapAddr, uint32_t aMultibootMmapLength);

/**
 * Activate given page directory
 * @param aVirtualAddr - virtual address of the page directory
 * @param aPhysicalAddr - physical address of the page directory
 */
void Paging_set_page_directory(uintptr_t aVirtualAddr, uintptr_t aPhysicalAddr);

/**
 * Register the page table entries database
 * @param aVirtualAddr - virtual address of the pte database
 * @param aPhysicalAddr - physical address of the pte database
 * @param aVirtualPteAddrOffset - virtual directory address of the pte database
 */
void Paging_set_pte_database(uintptr_t aVirtualAddr,
                             uintptr_t aPhysicalAddr,
                             uintptr_t aVirtualPteAddrOffset);

/**
 * Get the page directory address
 * @return virtual address
 */
uintptr_t Paging_get_page_directory();

/**
 * Get the page table entry database address
 * @return virtual address
 */
uintptr_t Paging_get_pte_database();

/**
 * Add a new page table entry to support a new virtual address space
 * @param aTargetVirtualAddr - the targeted virtual address space to manage
 * @param aPhysicalAddr - a physical page to use for the pte
 */
void Paging_add_pte(uintptr_t aTargetVirtualAddr, uintptr_t aPhysicalAddr);

/**
 * Map a virtual page to a physical page
 * @param aPhysicalAddr - the target physical page
 * @param aVirtualAddr - the target virtual page
 * @param somePageFlags - the pte flags to apply
 * @param anOverrideFlag - a flag for overriding an existing mapping
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
uint32_t Paging_map(uintptr_t aPhysicalAddr,
                    uintptr_t aVirtualAddr,
                    uint32_t  somePageFlags,
                    bool      anOverrideFlag);

/**
 * Lookup the physical address associated with the given virtual address
 * @param aVirtualAddr - any virtual address
 * @return physical address
 */
uintptr_t Paging_find_physical_address(uintptr_t aVirtualAddr);

#endif // I386_VIRTUAL_H
