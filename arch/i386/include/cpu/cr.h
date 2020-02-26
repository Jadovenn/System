/**
 * cr.h - CPU Control Registers
 * System sources under license MIT
 */

#ifndef CR_H_
# define CR_H_

#include <stdint.h>

uint32_t	read_cr3();
void		write_cr3(uint32_t cr3);
uint32_t	read_cr2();

/**
 * @brief flush tlb, make CR3 reloaded and new page validated
 */
void	flush_tlb();

#endif // CR_H_

