/**
 * mmu_init.c - Memory Management Unit Installation
 * System under license MIT
 */

#include <stdlib.h>
#include <string.h>
#include <system.h>
#include <multiboot.h>
#include <kernel/stdio.h>
#include <kernel/heap.h>
#include <kernel/panic.h>

#include "cpu/mmu.h"
#include "cpu/cr.h"
#include "arch/paging.h"
#include "arch/memlayout.h"

/**
 * Step to initialize paging
 * 1. install boot page directory and page table
 * 2. mark .text section of the kernel as read-only
 * 3. mark .rodata section of the kernel as read-only
 * 4. flush the TLB to make change effective
 * 5. setup necessarly free space right after the kernel
 */

//
// Post boot - Pre init sequence
//

static void	__set_section_text_ro() {
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_code);
	uint32_t vaddr = (uint32_t)&_kernel_start;
	uint32_t paddr = VIRTUAL_ADDR_TO_PHYSICAL(&_kernel_start);
	unsigned page_mapped = 0;
	for (;paddr < text_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		paging_map_physical(paddr, vaddr, 0x001);
		page_mapped += 1;
	}
}

static void	__set_section_rodata_ro() {
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_rodata);
	uint32_t vaddr = (uint32_t)&_rodata;
	uint32_t paddr = VIRTUAL_ADDR_TO_PHYSICAL(&_rodata);
	unsigned page_mapped = 0;
	for (;paddr < rodata_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		paging_map_physical(paddr, vaddr, 0x001);
		page_mapped += 1;
	}
}

void	boot_paging_init() {
	mmu.boot_page_directory = &boot_page_directory;
	mmu.boot_page_table = &boot_page_table;
	__set_section_text_ro();
	__set_section_rodata_ro();
	flush_tlb();
}

//
// Post Init sequence - Pre kernel cold and dark startup
//

static int	*kernel_page_directory = NULL;

/**
 * @brief allocate physical memory for kernel page directory
 * @return valide virtual ptr to page the directory
 */
static __inline__ void	*__setup_free_space_for_pgdt() {
	int physic_kpd_start = VIRTUAL_ADDR_TO_PHYSICAL(&_end);
	if (physic_kpd_start % 0x1000) {
		physic_kpd_start += 0x1000 - physic_kpd_start % 0x1000;
	}
	int page_offset = physic_kpd_start / 0x1000;
	int pte_entry = mmu.boot_page_table[page_offset] & 0xFFFF0000;
	mmu.boot_page_table[page_offset] = pte_entry | 0x001;
	flush_tlb();
	return (void*)PHYSICAL_ADDR_TO_VIRTUAL(physic_kpd_start);
}

void	kernel_paging_init(multiboot_info *header) {
	(void)header;
	//kernel_page_directory = __setup_free_space_for_pgdt();
	//printk("kpd_start addr: %#x\n", &kernel_page_directory);
	//memset(kernel_page_directory, 0x100, 0);
}

/**
 * dirty code, keep it here

 * @details configure kernel heap just after the physical
 * space of the kernel. But in virtual space the heap is before the kernel

static void	configure_kernel_heap() {
	const size_t page_size = get_page_size();
	mmu.heap.placement_address = VIRTUAL_ADDR_TO_PHYSICAL(&_end);
	if ((mmu.heap.placement_address % page_size) != 0) {
		mmu.heap.placement_address += (0x1000 - (mmu.heap.placement_address % page_size));
	}
	mmu.heap.page_count = 0x100;
	install_heap((void*)PHYSICAL_ADDR_TO_VIRTUAL(mmu.heap.placement_address), (void*)mmu.heap.placement_address, 0x100 * 0x1000);
}
*/

