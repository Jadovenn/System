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

void		write_cr3(uint32_t cr3) {
	__asm__("mov %%eax, %%cr3" : : "a" (cr3));
}

uint32_t	read_cr2() {
	uint32_t	cr2 = 0;
	__asm__("mov %%cr2, %0" : "=a" (cr2) : );
	return cr2;
}

