/**
 * cr.c - CPU Control Register
 * System sources under license MIT
 */

#include <stdint.h>

void	flush_tlb() {
	__asm__("mov %%cr3, %%eax"::);
	__asm__("mov %%eax, %%cr3"::);
}

uint32_t	read_cr3() {
	uint32_t	cr3 = 0;
	__asm__("mov %%cr3, %0" : "=a" (cr3) : );
	return cr3;
}

