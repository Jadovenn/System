/**
 * frame.c - Frame allocation and bitset arithmetic
 * System sources under license MIT
 */

// TODO: this file absolutly require more comment and deep explanation

#include <system.h>
#include <string.h>
#include <cpu/isr.h>
#include <kernel/heap.h>
#include <kernel/paging.h>
#include <kernel/types.h>

uint32_t	*frames;
uint32_t	n_frames;

// defined in heap.c
extern uint32_t	placement_address;

page_directory_t	*kernel_page_directories = NULL;
page_directory_t	*current_page_directory = NULL;

#define INDEX_FROM_BIT(x)	(x / (8 * 4))
#define OFFSET_FROM_BIT(x)	(x % (8 * 4))

/**
 * @details static func to set a bit in the frames bitsets
 */
static void	set_frame(uint32_t	frame_addr) {
	uint32_t	frame = frame_addr / 0x1000;
	uint32_t	idx = INDEX_FROM_BIT(frame);
	uint32_t	off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

/**
 * @details static func to clear a bit int the frames bitsets
 */
static void	clear_frame(uint32_t	frame_addr) {
	uint32_t	frame = frame_addr / 0x1000;
	uint32_t	idx = INDEX_FROM_BIT(frame);
	uint32_t	off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

/**
 * @details test if a bit is set in the frames bitsets
 */
static bool	is_set_frames(uint32_t	frame_addr) {
	uint32_t	frame = frame_addr / 0x1000;
	uint32_t	idx = INDEX_FROM_BIT(frame);
	uint32_t	off = OFFSET_FROM_BIT(frame);
	return !((frames[idx] & (0x1 << off)) == 0);
}

static uint32_t		first_available_frame() {
	for (uint32_t i = 0; i < INDEX_FROM_BIT(n_frames); i++) {
		if (frames[i] != 0xffffffff) {
			for (uint32_t j = 0; j < 32; j++) {
				uint32_t to_test = (0x1 << j);
				if (!(frames[i] & to_test)) {
					return i * 4 * 8 + j;
				}
			}
		}
	}
}

void	alloc_frame(page_t *page, int is_kernel, int is_writeable) {
	if (page->frame != 0) { // Special case: page already have a frame
		return;
	}
	else { // Regular case: give the page a frame
		uint32_t	idx = first_available_frame();
		if (idx == (uint32_t)-1) { // Special case: no available frame
			PANIC("No available frames.");
		}
		set_frame(idx * 0x1000); // take the address not the idx
		page->present = 1;
		page->rw = (is_writeable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = idx;
	}
}

void	free_frame(page_t *page) {
	uint32_t	frame;
	if (!(frame = page->frame)) {
		return;
	}
	else {
		clear_frame(frame);
		page->frame = 0x0;
	}
}

// https://wiki.osdev.org/Paging
/**
 * @details initialize paging, carefull with kmalloc, block
 * could not be free, no need to check the return value it can not failed
 * memo: at this point placement_address point to the end (defined by the linker)
 * plus the non freeable space already allocated, so {0x0-placement_address} is kernel space
 */
void	init_paging() {
	uint32_t	mem_end_page = 0x1000000; // 16Mb
	n_frames = mem_end_page / 0x1000;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(n_frames));
	memset(frames, 0, INDEX_FROM_BIT(n_frames));

	kernel_page_directories = (page_directory_t*)kmalloc_aligned(sizeof(page_directory_t));
	//memset(kernel_page_directories, 0, sizeof(page_directory_t));
	current_page_directory = kernel_page_directories;
	// here come some tricky stuff TODO: Review this, memo: while loop must be used
	int i = 0;
	// map all the kernel space to a readable region but not writeable from userspace
	while (i < placement_address) { // read the memo in the comment header function
		alloc_frame(get_page(i, 1, kernel_page_directories), 0, 0);
		i += 0x1000;
	}
	register_interrupt_handler(14, page_fault);
	switch_page_directory(kernel_page_directories);
}

void	switch_page_directory(page_directory_t *dir) {
	current_page_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tables_physical_address));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0)); // enable pagin here
}

page_t	*get_page(uint32_t address, int make, page_directory_t *dir) {
	address /= 0x1000;
	uint32_t table_idx = address / 1024;
	if (dir->tables[table_idx]) { // Regular Case: the page exist
		return &dir->tables[table_idx]->pages[address % 1024];
	}
	else if (make) { // Regular Case: the page does not exist we must create it
		uint32_t tmp;
		dir->tables[table_idx] = (page_table_t*)kmalloc_physical_align(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tables_physical_address[table_idx] = tmp | 0x7; // is present, rw, user
		return &dir->tables[table_idx]->pages[address % 1024];
	}
	else { // Special Case: the page does not exist at all
		return NULL;
	}
}

void	page_fault(registers_t regs) {
	uint32_t	fault_address;
	asm volatile("mov %%cr2, %0" : "=r" (fault_address));

	int present = !(regs.err_code & 0x1);
	int rw = regs.err_code & 0x2;
	int us = regs.err_code & 0x4;
	int reserved = regs.err_code & 0x8;
	int id = regs.err_code & 0x10;
	
	printk("Page fault interruption (");
	if (present) {
		printk("present, ");
	}
	else {
		printk("not present ,");
	}
	if (rw) {
		printk("read only, ");
	}
	else {
		printk("read and write, ");
	}
	if (us) {
		printk("user-mode, ");
	}
	else {
		printk("kernel-mode, ");
	}
	if (reserved) {
		printk("reserved");
	}
	else {
		printk("not reserved");
	}
}

