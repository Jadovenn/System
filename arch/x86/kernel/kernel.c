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

void	check_multiboot(multiboot_info *header, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("Wrong multiboot magic number, unable to perform  memory mapping.");
	}
}

/**
 * @details kernel multiboot entrypoint
 */
void	kernel_main_multiboot(multiboot_info *mltb_header, uint32_t magic) {
	config_cpu();
	check_multiboot(mltb_header, magic);
	monitor_init();
	init_timer(50);
	while(1);
}

