/**
 * paging.h - paging related function
 * System sources under license MIT
 */

#ifndef PAGING_H_
# define PAGING_H_

#include <stdbool.h>
#include <stdint.h>
#include <api/mm.h>
#include <cpu/isr.h> 
/**
 * @brief Map a physical page to a virtual page
 * @param physcial_addr - aligned physical page addr
 * @param virtual_addr - aligned virtual page addr
 * @param flags - page configuration flags
 * @param override - if true override existing setting if there is a present valide page
 */
uint32_t pg_map(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags, bool override);

/**
 * @brief add a new PTE
 * @details Do not use until kernel init is own page directory.
 * 	The pte will be cleared for you and tlb flushed
 * @param vaddr - virtual addr space that the pte will manage
 * @param paddr - physical addr of the page ti use as a pte
 */
void	pg_add_pte(uint32_t vaddr, uint32_t paddr);

/**
 * @brief boot page fault handler
 */
void boot_page_fault_handler(registers_t regs);

/**
 * @brief translate a virtual address to physical addr
 * @param virtual_addr - virtual address to translate, not
 * 	necessarly aligned
 * @return {uint32_t} - corresponding physical address
 */
uint32_t pg_translate_virtual(uint32_t virtual_addr);

extern vma_struct_t	_kernel_vma[];

#endif // PAGING_H_

