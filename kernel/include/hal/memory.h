/**
 * memory.h - Kernel Memory Management API
 * System sources under license MIT
 */

#ifndef HAL_MEMORY_H_
#define HAL_MEMORY_H_

#include <stddef.h>
#include <stdint.h>

/********************************
 **     Structure and Enum     **
 ********************************/

typedef enum Hal_memory_page_flag {
	OWNER     = 0x01,
	ACCESS    = 0x02,
	ANONYMOUS = 0x04,
} Hal_memory_page_flag_e;

typedef enum Hal_memory_access_flag {
	READ_ONLY  = 0,
	READ_WRITE = 1
} Hal_memory_access_flag_e;

typedef struct Hal_memory_page { // to delete
	uintptr_t* virtualAddr;
	uintptr_t* physicalAddr;
	uint8_t    flags;
} Hal_memory_page_t;

typedef struct Hal_memory_area { // to delete
	struct Hal_memory_page* pages;
	size_t                  pageCount;
} Hal_memory_area_t;

typedef struct Hal_memory_region {
	uintptr_t                start;
	uintptr_t                end;
	Hal_memory_access_flag_e flag;
} Hal_memory_region_t;

/********************************
 **  Public functions          **
 ********************************/

size_t Hal_get_page_size();

/**
 * Set the heap break address
 * @param anAddress - a target address
 * @return EXIT_FAILURE or EXIT_SUCCESS if request is not valid
 */
int   Hal_brk(void* anAddress);

/**
 * increase the heap, return the previous break address
 * @param aSize - a requested size
 * @return return the previous break address
 */
void* Hal_sbrk(size_t aSize);

#endif // HAL_MEMORY_H_
