/**
 * mmu.c - Memory Management Unit
 * System under license MIT
 */

#include <system.h>
#include <cpu/mmu.h>

struct mmu_t	mmu = { 
	NULL,
	NULL,
	0,
	{
		NULL,
		NULL,
		0,
		0
	}
};

size_t	get_page_size() {
	return mmu.page_size;
}

