/**
 * mmu.h - Memory Management Unit
 * System under license MIT
 */

#ifndef MMU_H_
# define MMU_H_

/**
 * @brief Memory Management Unit
 * @details mmu c object is responsible of free aera mapping, 
 * address translation and placement address
 */

#include <kernel/types.h>
#include <kernel/multiboot.h>

struct mmu_t	{
	uint32_t	*boot_page_directory;
	uint32_t	*boot_page_table;
};

extern struct mmu_t	mmu;


#endif // MMU_H_

