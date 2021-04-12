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

typedef enum Virtual_memory_type
{
	vmt_USERSPACE    = 0x0001,
	vmt_BIOS         = 0x0002,
	vmt_RESERVED     = 0x0004,
	vmt_KERNEL_IMAGE = 0x0008,
	vmt_LOW_MEMORY   = 0x0010,
	vmt_HEAP         = 0x0020,
	vmt_DATA         = 0x0040,
	vmt_DAEMON       = 0x0080,
	vmt_END          = 0x0100,
} Virtual_memory_type_e;

typedef struct Virtual_memory_area
{
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
void Paging_set_pte_database(uintptr_t aVirtualAddr, uintptr_t aPhysicalAddr, uintptr_t aVirtualPteAddrOffset);

/**
 * Request a pte for the given virtual address
 * @param aVirtualAddr - a 4Kib aligned virtual address
 * @return - NULL if no pte found or the virtual address of the pte
 */
uint32_t* Paging_request_from_pte_database(uintptr_t aVirtualAddr);
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
 * Set page attributes
 * @param aVirtualAddr - 4Kib aligned virtual address
 * @param aPhysicalAddr - Physical address to map
 * @param someFlags - paging flags
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int Paging_add_page_entry(uintptr_t aVirtualAddr, uintptr_t aPhysicalAddr, uint32_t someFlags);

/**
 * Clear page attributes
 * @param aVirtualAddr - 4Kib aligned virtual address
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int Paging_clear_page_entry(uintptr_t aVirtualAddr);

/**
 * Update page attributes
 * @param aVirtualAddr - 4Kib aligned virtual address
 * @param flags - flags on the first 12 bits, if the bitset is overflowing it
 * 								fails
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int Paging_update_page_entry_flags(uint32_t aVirtualAddr, uint32_t flags);

/**
 * Lookup the physical address associated with the given virtual address
 * @param aVirtualAddr - any virtual address
 * @return physical address
 */
void* Paging_find_physical_address(uintptr_t aVirtualAddr);

#endif // I386_VIRTUAL_H
