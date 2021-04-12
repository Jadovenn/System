/**
 * memory.h - Kernel Memory Management API
 * System sources under license MIT
 */

#ifndef HAL_MEMORY_H_
#define HAL_MEMORY_H_

#include <stddef.h>
#include <stdint.h>

#define MAP_FAILED ((void*)-1)

/********************************
 **     Structure and Enum     **
 ********************************/

typedef enum Hal_memory_mode
{
	map_READWRITE = 0x0001,
	map_READONLY  = 0x0002,
	map_EXECUTE   = 0x0004,
	map_ANONYMOUS = 0x0008,
	map_POPULATE  = 0x0010,
	map_DATA      = 0x0020, // default
} Hal_memory_mode_e;

/********************************
 **  Public functions          **
 ********************************/

size_t Hal_get_page_size();

/**
 * Set the heap break address
 * @param anAddress - a target address
 * @return EXIT_FAILURE or EXIT_SUCCESS if request is not valid
 */
int Hal_brk(void* anAddress);

/**
 * increase the heap, return the previous break address
 * @param aSize - a requested size
 * @return return the previous break address
 */
void* Hal_sbrk(size_t aSize);

/**
 * Map physical memory to virtual memory
 * @param aPhysicalAddr - 4Kib aligned physical address
 * @param aPageCount - the number of contiguous page to map
 * @param aMode - some map modes
 * @return NULL or a usable pointer
 */
void* Hal_mmap(uintptr_t aPhysicalAddr, size_t aPageCount, Hal_memory_mode_e aMode);

/**
 * Clear memory mapping
 * @param aVirtualAddr - 4Kib aligned virtual address
 * @param aPageCount - the number of contiguous page to unmap
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int Hal_munmap(void* aVirtualAddr, size_t aPageCount);

/**
 * Retrieve an associated physical memory address
 * @param aVirtualAddr - any mapped virtual address
 * @return MAP_FAILED or a pointer to the physical address
 */
void* Hal_get_associated_addr(uintptr_t aVirtualAddr);

#endif // HAL_MEMORY_H_
