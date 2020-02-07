/**
 * paging.h - paging related function
 * System sources under license MIT
 */

#ifndef PAGING_H_
# define PAGING_H_

#include <stdbool.h>
#include <stdint.h>
#include <cpu/isr.h>

/**
 * @brief map a physical page to a virtual page
 * @param physcial_addr - aligned physical page addr
 * @param virtual_addr - aligned virtual page addr
 * @param flags - page configuration flags
 * @param override - if true override existing setting if there is a present valide page
 */
uint32_t paging_map_physical(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags, bool override);

/**
 * @brief page fault handler
 */
void page_fault_handler(registers_t regs);

/**
 * @brief translate a virtual address to physical addr
 * @param virtual_addr - virtual address to translate, not
 * 	necessarly aligned
 * @return {uint32_t} - corresponding physical address
 */
uint32_t paging_translate_virtual(uint32_t virtual_addr);

#endif // PAGING_H_

