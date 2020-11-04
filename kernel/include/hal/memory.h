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

void      Hal_init_memory_allocator(uintptr_t vstart, size_t size);
uintptr_t Hal_increase_heap_break(size_t aSize);
uintptr_t Hal_set_break(uintptr_t aBreakAddr);

Hal_memory_area_t* Hal_alloc_pages(size_t count, size_t flags);

void Hal_free_page(Hal_memory_page_t* page);
void Hal_free_pages(Hal_memory_area_t* area);

Hal_memory_area_t* Hal_mmap(uintptr_t                aPhysicalAddr,
                            uintptr_t                aVirtualAddr,
                            Hal_memory_access_flag_e aFlag);
void               Hal_unmap(Hal_memory_area_t* area);

#endif // HAL_MEMORY_H_
