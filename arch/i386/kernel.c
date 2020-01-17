/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <kernel/panic.h>
#include <multiboot.h>

#include "arch/init.h"

extern void main(int ac, char **av);

/**
 * @brief install CPU related unit
 */
void	cpu_init() {
	gdt_init();
	idt_init();
	boot_paging_init();
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
	if (!MULTIBOOT_TEST_FLAG(header, MULTIBOOT_INFO_MEMORY)) {
		PANIC("Multiboot memmory flags is not present, \
				manual memory detection is not available");
	}
	if (!(header->flags & MULTIBOOT_INFO_MMAP)) {
		PANIC("Multiboot memory map is not present, \
				manual memory detection is not available");
	}
	printk("Lower memory region size: %dKiB\n", header->mem_lower);
	printk("Upper memory region size: %dKiB\n", header->mem_upper);
}

/**
 * @brief kernel entrypoint
 * @details the order init function are called matter a lot
 */
void	kmain(multiboot_info *header, uint32_t magic) {
	cpu_init();
	drivers_init();
	check_multiboot(header, magic);
	kernel_paging_init(header);
	physical_memory_init(header);
	main(0, NULL);
}

