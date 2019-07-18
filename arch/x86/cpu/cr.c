/**
 * cr.c - CPU Control Register
 * System sources under license MIT
 */

void	flush_tlb() {
	__asm__("mov %%cr3, %%eax"::);
	__asm__("mov %%eax, %%cr3"::);
}

