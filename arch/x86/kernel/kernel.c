/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/multiboot.h>
#include <kernel/timer.h>

#include "init/init.h"

/**
 * @details install global descriptors tables and
 * interrupt descriptor table
 */
void	config_cpu() {
	install_gdt();
	install_idt();
	asm volatile ("sti");
}

/**
 * multiboot info header parser
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
	printk("Lower Memory: %dKiB\n", header->mem_lower);
	printk("Upper Memory: %dKiB\n", header->mem_upper);
}

/**
 * @details kernel multiboot entrypoint
 */
void	kmain(multiboot_info *header, uint32_t magic) {
	config_cpu();
	monitor_init();
	init_with_multiboot(header, magic);
	init_timer(50);
	while(1);
}

