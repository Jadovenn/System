/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/multiboot.h>
#include <kernel/timer.h>

#include "init/init.h"

/**
 * @brief install CPU related unit
 */
void	config_cpu() {
	install_gdt();
	install_idt();
	install_mmu();
	asm volatile ("sti");
}

/**
 * multiboot info header parser
 */
void	check_multiboot(multiboot_info *header, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("Wrong multiboot magic number, \
				multiboot header is not present");
	}
	if (!(header->flags & MULTIBOOT_INFO_MEMORY)) {
		PANIC("Multiboot memmory flags is not present, \
				manual memory detection is not available");
	}
	if (!(header->flags & MULTIBOOT_INFO_MMAP)) {
		PANIC("Multiboot memory map is not present, \
				manual memory detection is not available");
	}
	printk("Lower Memory Size: %dKiB\n", header->mem_lower);
	printk("Upper Memory Size: %dKiB\n", header->mem_upper);
}

/**
 * @brief kernel entrypoint
 * @details the order init function are called matter a lot
 */
void	kmain(multiboot_info *header, uint32_t magic) {
	config_cpu();
	monitor_init();
	check_multiboot(header, magic);
	configure_mmu(header);
	init_timer(50);
	while(1);
}

