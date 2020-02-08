/**
 * memlayout.h - Physical Memory Layout
 * System under license MIT
 */

#ifndef MEMLAYOUT_H_
# define MEMLAYOUT_H_

#include <stdint.h>

#define VIRTUAL_KERNEL_SPACE_ADDR 0xC0000000

#define VIRTUAL_PTR_TO_PHYSICAL(x)	(uint32_t*)(((uint32_t)x) - VIRTUAL_KERNEL_SPACE_ADDR)
#define PHYSICAL_PTR_TO_VIRTUAL(x)	(uint32_t*)(((uint32_t)x) + VIRTUAL_KERNEL_SPACE_ADDR)

#define VIRTUAL_ADDR_TO_PHYSICAL(x)	(uint32_t)(((uint32_t)x) - VIRTUAL_KERNEL_SPACE_ADDR)
#define PHYSICAL_ADDR_TO_VIRTUAL(x)	(uint32_t)(((uint32_t)x) + VIRTUAL_KERNEL_SPACE_ADDR)

/**
 * Globale variable here are in order they shoul be in memory
 */

/**
 * Defined in ld script
 */
extern uint32_t _kernel_space;
extern uint32_t	_kernel_start;
extern uint32_t	boot_page_directory; // only available at boot time
extern uint32_t	boot_page_table; // only available at boot time
extern uint32_t	_code;
extern uint32_t	_end_code;
extern uint32_t	_rodata;
extern uint32_t	_end_rodata;
extern uint32_t	_end_data;
extern uint32_t	_bss;
extern uint32_t	_end;

/**
 * Managed by physical memory manager
 */
extern uint32_t _physical_mmap_start;
extern uint32_t _physical_mmap_end;

/**
 * Managed by virtual memory manager
 */
extern uint32_t	_page_directory_start;
extern uint32_t	_page_directory_end;
extern uint32_t _page_table_entries_start;
extern uint32_t _page_table_entries_end;

extern uint32_t _DMA_memory_region_start;
extern uint32_t	_DMA_memory_region_end; // paddr: 0x01000000

#endif // MEMLAYOUT_H_

