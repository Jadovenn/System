/**
 * mmu_init.c - Memory Management Unit Installation
 * System sources under license MIT
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <multiboot.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>

#include <cpu/cr.h>
#include <arch/paging.h>
#include <arch/memlayout.h>

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

static __inline__ 
void	__set_section_text_ro() {
	uint32_t text_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_code);
	uint32_t vaddr = (uint32_t)&_kernel_start;
	uint32_t paddr = VIRTUAL_ADDR_TO_PHYSICAL(&_kernel_start);
	unsigned page_mapped = 0;
	for (;paddr < text_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		pg_map_physical(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

static __inline__
void	__set_section_rodata_ro() {
	uint32_t rodata_end_addr = VIRTUAL_ADDR_TO_PHYSICAL(&_end_rodata);
	uint32_t vaddr = (uint32_t)&_rodata;
	uint32_t paddr = VIRTUAL_ADDR_TO_PHYSICAL(&_rodata);
	unsigned page_mapped = 0;
	for (;paddr < rodata_end_addr; vaddr += 0x1000, paddr += 0x1000) {
		pg_map_physical(paddr, vaddr, 0x001, true);
		page_mapped += 1;
	}
}

/**
 * Paging init sequence:
 *	1. allocate & init physical page for page_table_entries
 *	2. allocate & init physical page for page_boot_directory
 *	3. flush tlb
 *	// recycle boot pg directory page, optional 
 *	4. set correct r/w access for text section
 *	5. set correct r/w access for rodata section
 */

uint32_t _page_directory_start;
uint32_t _page_directory_end;
uint32_t _page_table_entries_start;
uint32_t _page_table_entries_end;

/**
 * Require by libc, sbrk*'s like procedure
 * but gdt is not affected
 */
static
void	*alloc_heap_space(size_t *size, size_t request) {
	return NULL;
}

/**
 * @brief init libc malloc, recycle boot pg directory
 */
void	heap_init() {
	memset(PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)&boot_page_directory), 0x1000, 0x0);
	
	libc_init_allocator(0xD0000000, 0x1000, &alloc_heap_space);
}

static void	_vmap_page_directory() {
	// alloc page contiguously
	_page_directory_start = _page_table_entries_end;
	_page_directory_end = _page_directory_start + 0x1000;
	uint32_t *vstart = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)_page_directory_start);
	uint32_t *pg_dir = PHYSICAL_PTR_TO_VIRTUAL(read_cr3());
	pg_map_physical(_page_directory_start, (uint32_t)vstart, 0x003, false);
	// init by copy of the boot page directory and remove itself from the boot table
	memcpy(vstart, pg_dir, 0x1000);
}

static void	_vmap_page_tables_entries() {
	// alloc page contiguously
	_page_table_entries_start = _physical_mmap_end;
	_page_table_entries_end = _page_table_entries_start + 0x1000;
	uint32_t *vstart = PHYSICAL_PTR_TO_VIRTUAL((uint32_t*)_page_table_entries_start);
	pg_map_physical(_page_table_entries_start, (uint32_t)vstart, 0x003, false);
	// init by adding manually the boot page directory page
	memset(vstart, 0x1000, 0x0);
	unsigned offset = PHYSICAL_ADDR_TO_VIRTUAL((uint32_t)&boot_page_table) >> 22;
	vstart[offset] = (uint32_t)&boot_page_table | 0x003;
}

void	paging_init(multiboot_info *header) {
	(void)header;
	_vmap_page_tables_entries();
	_vmap_page_directory();
	write_cr3(_page_directory_start);
	flush_tlb();
	
	__set_section_text_ro();
	__set_section_rodata_ro();
	//heap_init();
}

