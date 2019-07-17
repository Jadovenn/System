/**
 * mmu_init.c - Memory Management Unit Installation
 * System under license MIT
 */

#include <system.h>
#include <kernel/stdio.h>
#include <kernel/memlayout.h>
#include <cpu/mmu.h>

static void	__set_section_text_ro() {
	uint32_t text_start_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_kernel_start);
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_code);
	uint32_t page_offset = text_start_addr / 0x1000;
	uint32_t pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	while (pte_addr < text_end_addr) {
		mmu.boot_page_table[page_offset] = pte_addr | 0x001;
		page_offset += 1;
		pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	}
}

static void	__set_section_rodata_ro() {
	uint32_t rodata_start_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_rodata);
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_rodata);
	uint32_t page_offset = rodata_start_addr / 0x1000;
	uint32_t pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000;
	while (pte_addr < rodata_end_addr) {
		mmu.boot_page_table[page_offset] = pte_addr | 0x001;
		page_offset += 1;
		pte_addr = mmu.boot_page_table[page_offset] & 0xFFFFF000; 
	}
}

// TODO: init global mmu struct, retrieve boot page directory and PTE
// mark already reserved area
void	install_mmu() {
	if (mmu.boot_page_directory != NULL) // mmu already init
		return;
	mmu.boot_page_directory = &boot_page_directory;
	mmu.boot_page_table = &boot_page_table;
	__set_section_text_ro();
	__set_section_rodata_ro();
	__asm__("mov %%cr3, %%eax" : : );
	__asm__("mov %%eax, %%cr3" : : );
}

void	read_multiboot_info_memory_map(multiboot_info *header) {
	printk("mmap_addr %#x\n", header->mmap_addr);
	printk("mmap_length %#x\n", header->mmap_length);
	multiboot_mmap_info *mmap;
	for (mmap = (multiboot_mmap_info*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap = (multiboot_mmap_info*)((uint32_t) mmap + mmap->size + sizeof(mmap->size)))
	{
		printk("mmap entry size: %d, addr: %l, len: %l bytes, type: %d\n", mmap->size, mmap->addr, mmap->len, mmap->type);
	}
}

// TODO: map free region using the multiboot header
// the flags as already been tested and memory fields are
// guaranty to be safe to read
void	configure_mmu(multiboot_info *header) {
	read_multiboot_info_memory_map(header);
}

