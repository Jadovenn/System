/**
 * boot_memory.h - Boot time memory management
 * System sources under license MIT
 */

#ifndef BOOT_MEMORY_H
#define BOOT_MEMORY_H

#include <stdint.h>

void     Boot_paging_add_pte(uintptr_t vaddr, uintptr_t paddr);
uint32_t Boot_paging_map(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags, bool override);

/*************************************
 ** init code variables declaration **
 *************************************/

extern uint32_t G_Boot_page_directory;
extern uint32_t G_Boot_page_table;

#endif // BOOT_MEMORY_H
