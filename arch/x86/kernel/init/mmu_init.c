/**
 * mmu_init.c - Memory Management Unit Installation
 * System under license MIT
 */

#include <system.h>
#include <kernel/memlayout.h>
#include <cpu/mmu.h>

// TODO: init global mmu struct, retrieve boot page directory and PTE
// mark already reserved area
void	install_mmu() {
	if (mmu.boot_page_directory != NULL) // mmu already init
		return;
	mmu.boot_page_directory = &boot_page_directory;
	mmu.boot_page_table = &boot_page_table;
}

// TODO: map free region using the multiboot header
// the flags as already been tested and memory fields are
// guaranty to be safe to read
void	configure_mmu(multiboot_info *header) {
	(void)header;
}

