/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/timer.h>

#include "init/init.h"

/**
 * @details install global descriptors tables and
 * interrupt descriptor table
 */
void	config_cpu() {
	install_gdt();
	install_idt();
}

/**
 * @details kernel early stage main entry point
 */
void	kernel_main() {
	config_cpu();
	monitor_init();
	// enable interruption
	asm volatile ("sti");
	// install timer tick handler for PIT 0 at 50hz
	init_timer(50);
	// should trigger a page fault if paging as been correctly setup
	uint32_t	*ptr = (uint32_t*)0xa0000000;
	uint32_t	page_fault = *ptr;
	(void)page_fault;
}

/**
 * @details kernel multiboot entrypoint
 */
void	kernel_main_multiboot() {
	config_cpu();
	monitor_init();
	asm volatile ("sti");
	printk("Hello multiboot header\n");
	init_timer(50);
	while(1);
}

