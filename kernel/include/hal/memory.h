/**
 * memory.h - Kernel Memory Management API
 * System sources under license MIT
 */

#ifndef HAL_MEMORY_H_
#define HAL_MEMORY_H_

#include <stdint.h>
#include <stddef.h>

typedef enum Hal_memory_page_flag {
	OWNER     = 0x01,
	ACCESS    = 0x02,
	ANONYMOUS = 0x04,
} Hal_memory_page_flag_e;

typedef enum Hal_memory_access_flag {
	READ_ONLY  = 0,
	READ_WRITE = 1
} Hal_memory_access_flag_e;

typedef struct Hal_memory_page {
	uintptr_t* virtualAddr;
	uintptr_t* physicalAddr;
	uint8_t    flags;
} Hal_memory_page_t;

typedef struct Hal_memory_area {
	struct Hal_memory_page* pages;
	size_t                  pageCount;
} Hal_memory_area_t;

size_t Hal_get_page_size();

Hal_memory_area_t* Hal_alloc_pages(size_t count, size_t flags);

void Hal_free_page(Hal_memory_page_t* page);
void Hal_free_pages(Hal_memory_area_t* area);

Hal_memory_area_t* Hal_mmap(uintptr_t physicalAddr, uintptr_t* virtualAddr);
void               Hal_unmap(Hal_memory_area_t* area);

void Hal_init_memory_allocator(uintptr_t vstart, size_t size);

#endif // HAL_MEMORY_H_
