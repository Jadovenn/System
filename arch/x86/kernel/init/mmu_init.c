/**
 * mmu_init.c - Memory Management Unit Installation
 * System under license MIT
 */

#include <system.h>
#include <kernel/stdio.h>
#include <kernel/memlayout.h>
#include <cpu/mmu.h>

void	_set_section_text_ro() {
	uint32_t text_start_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_kernel_start);
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&ecode);
	uint32_t page_offset = text_start_addr / 0x1000;
	uint32_t pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	while (pte_addr < text_end_addr) {
		mmu.boot_page_table[page_offset] = pte_addr | 0x001;
		page_offset += 1;
		pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	}
	__asm__("mov %%cr3, %%eax" : : );
	__asm__("mov %%eax, %%cr3" : : );
}

// TODO: init global mmu struct, retrieve boot page directory and PTE
// mark already reserved area
void	install_mmu() {
	if (mmu.boot_page_directory != NULL) // mmu already init
		return;
	mmu.boot_page_directory = &boot_page_directory;
	mmu.boot_page_table = &boot_page_table;
	_set_section_text_ro();
}

// TODO: map free region using the multiboot header
// the flags as already been tested and memory fields are
// guaranty to be safe to read
void	configure_mmu(multiboot_info *header) {
	(void)header;
}

