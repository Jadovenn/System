/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <kernel/panic.h>
#include <kernel/multiboot.h>

#include "arch/init.h"

extern void main(int ac, char **av);

/**
 * @brief install CPU related unit
 */
void	cpu_init() {
	gdt_init();
	idt_init();
	paging_init();
	asm volatile ("sti");
}

void	drivers_init() {
	monitor_driver_init();
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
	cpu_init();
	drivers_init();
	check_multiboot(header, magic);
	main(0, NULL);
}

