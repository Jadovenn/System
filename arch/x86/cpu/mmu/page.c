/**
 * page.c - Kernel Heap Management
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/memlayout.h>
#include <kernel/heap.h>
#include <cpu/cr.h>
#include <cpu/mmu.h>

// TODO: actually we have a dummy physical memory management,
// and assum that this will never been free because
// kmalloc will keep the pages for futher allocation
// It would be better to use the save function for all
// page allocation
void	*get_new_heap_page(uint32_t *physical_addr) {
	const size_t page_size = get_page_size();
	uint32_t new_page = mmu.heap.placement_address + mmu.heap.page_count * page_size;
	if (physical_addr) {
		*physical_addr = new_page;
	}
	mmu.heap.page_count += 1;
	mmu.heap.top = PHYSICAL_PTR_TO_VIRTUAL(new_page + 0x1000);
	mmu.boot_page_table[new_page / page_size] = new_page | 0x003;
	flush_tlb();
	return PHYSICAL_PTR_TO_VIRTUAL(new_page);
}

void	*get_page(uint32_t *physical_addr, bool kernel, bool read_only) {
	//uint32_t	ph_addr = first_available_page();
	(void)physical_addr;
	(void)kernel;
	(void)read_only;
	return NULL;
}

