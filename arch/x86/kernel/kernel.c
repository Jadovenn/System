/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/timer.h>
#include <kernel/paging.h>

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
void	kmain() {
	config_cpu();
	init_paging();
	monitor_init();
	// enable interruption
	asm volatile ("sti");
	// install timer tick handler for PIT 0 at 50hz
	init_timer(50);
	// should trigger a page fault if paging as been correctly setup
	uint32_t	*ptr = (uint32_t*)0xa0000000;
	uint32_t	page_fault = *ptr;
}

