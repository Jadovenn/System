/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/multiboot.h>
#include <kernel/timer.h>
#include <cpu/mmu.h>

#include "init/init.h"

/**
 * @details install global descriptors tables and
 * interrupt descriptor table
 */
void	config_cpu() {
	install_gdt();
	install_idt();
	//enable_mmu();
	asm volatile ("sti");
}

/**
 * @details kernel early stage main entry point
 */
void	kernel_main() {
	config_cpu();
	monitor_init();
	// enable interruption
	// install timer tick handler for PIT 0 at 50hz
	init_timer(50);
	// should trigger a page fault if paging as been correctly setup
	//uint32_t	*ptr = (uint32_t*)0xa0000000;
	//uint32_t	page_fault = *ptr;
	//(void)page_fault;
}

/**
 * init mmu
 */
void	init_with_multiboot(multiboot_info *header, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("Wrong multiboot magic number, multiboot header is not present");
	}
	if (!(header->flags & MULTIBOOT_INFO_MEMORY)) {
		PANIC("Multiboot memmory flags is not present, manual memory detection is not available");
	}
	if (!(header->flags & MULTIBOOT_INFO_MMAP)) {
		PANIC("Multiboot memory map is not present, manual memory detection is not available");
	}
}

/**
 * @details kernel multiboot entrypoint
 */
void	kernel_main_multiboot(multiboot_info *header, uint32_t magic) {
	config_cpu();
	monitor_init();
	//init_with_multiboot(header, magic);
	//printk("Lower Memory: %dKiB\n", header->mem_lower);
	//printk("Upper Memory: %dKiB\n", header->mem_upper);
	init_timer(50);
	while(1);
}

