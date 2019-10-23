/**
 * mmu_init.c - Memory Management Unit Installation
 * System under license MIT
 */

#include <stdlib.h>
#include <system.h>
#include <kernel/stdio.h>
#include <kernel/memlayout.h>
#include <kernel/heap.h>
#include <kernel/panic.h>
#include <cpu/mmu.h>
#include <cpu/cr.h>

/**
 * Step to initialize memory management unit
 * 1. install mmu by marking read-only section in the boot page table
 * 2. initialize heap marker to startup the heap algorithm
 * 3. read and set physical frames from the multiboot header (require the heap working)
 * 4. mark physical frame reserved for devices, heap and kernel spaces
 * 5. register page fault interrupt handler
 */

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
	flush_tlb();
}

void	read_multiboot_info_memory_map(multiboot_info *header) {
	printk("mmap_addr %#x\n", header->mmap_addr);
	printk("mmap_length %#x\n", header->mmap_length);
	multiboot_mmap_info *mmap;
	for (mmap = (multiboot_mmap_info*)PHYSICAL_PTR_TO_VIRTUAL(header->mmap_addr);
			(uint32_t)mmap < PHYSICAL_ADDR_TO_VIRTUAL(header->mmap_addr) + header->mmap_length;
			mmap = (multiboot_mmap_info*)((uint32_t) mmap + mmap->size + sizeof(mmap->size)))
	{
		int result = map_frame_region((uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
		if (result == EXIT_FAILURE) {
			printk("Warning memory region not mapped\n");
		}
	}
}

/**
 * @brief mark reserved frames
 */
static void	mark_kernel_frames() {
	//frame	*iterator = mmu.frames;
}

static void	mark_heap_frames() {
}

static void	mark_devices_frames() {

}

/**
 * @details configure kernel heap just after the physical
 * space of the kernel. But in virtual space the heap is before the kernel
 */
static void	configure_kernel_heap() {
	const size_t page_size = get_page_size();
	mmu.heap.page_count = 0;
	mmu.heap.base = 0;
	mmu.heap.top = 0;
	mmu.heap.placement_address = VIRTUAL_ADDR_TO_PHYSICAL(&_end);
	if ((mmu.heap.placement_address % page_size) != 0) {
		mmu.heap.placement_address += (0x1000 - (mmu.heap.placement_address % page_size));
	}
}

// TODO: map free region using the multiboot header
// the flags as already been tested and memory fields are
// guaranty to be safe to read
void	configure_mmu(multiboot_info *header) {
	mmu.page_size = 0x1000;
	configure_kernel_heap();
	read_multiboot_info_memory_map(header);
	mark_kernel_frames();
	mark_heap_frames();
	mark_devices_frames();
	if (!mmu.frames) {
		PANIC("No physical frames as been read, could not manage memory.");
	}
	for (frame *iterator = mmu.frames; iterator; iterator = iterator->next) {
		printk("frame entry type: %d, addr: %#x, size: %d frames\n", iterator->type, iterator->physical_addr, iterator->size);
	}
}

