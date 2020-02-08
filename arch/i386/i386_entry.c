/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <kernel/panic.h>
#include <multiboot.h>

#include <arch/init.h>
#include <arch/paging.h>
#include <arch/hal.h>
#include <cpu/isr.h>

extern void main(int ac, char **av);

hal_t	hal;

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
void	i386_entry(multiboot_info *header, uint32_t magic) {
	gdt_init();
	idt_init();

	asm volatile ("sti");
	monitor_driver_init();

	check_multiboot(header, magic);
	register_interrupt_handler(14, &page_fault_handler);
	physical_memory_init(header);
	paging_init(header);
	main(0, NULL);
}

