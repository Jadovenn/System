/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <kernel/panic.h>
#include <multiboot.h>

#include <arch/init.h>
#include <arch/paging.h>
#include <cpu/isr.h>

extern void main(int ac, char **av);

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
 * @brief arch entrypoint called from
 * 	the multiboot header
 */
void	i386_entry(multiboot_info *header, uint32_t magic) {
	gdt_init();
	idt_init();
	asm volatile ("sti");
	register_interrupt_handler(14, &boot_page_fault_handler);
	monitor_driver_init();
	check_multiboot(header, magic);
	physical_memory_init(header);
	paging_init(header);
	uint32_t *integer = malloc(sizeof(uint32_t));
	printk("%#x\n", integer);
	main(0, NULL);
}

