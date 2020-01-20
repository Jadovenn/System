/**
 * paging.h - paging related function
 * System sources under license MIT
 */

#ifndef PAGING_H_
# define PAGING_H_

#include "cpu/isr.h"

void	paging_map_physical(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags);
void	page_fault_handler(registers_t regs);

#endif // PAGING_H_

