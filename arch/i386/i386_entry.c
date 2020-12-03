/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include <kernel/multiboot.h>
#include <kernel/panic.h>

#include <arch/init.h>
#include <arch/paging.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/isr.h>

#include "cpu/apic/apic.h"
#include "memory/virtual.h"

#include <system.h>

extern void main(int ac, char** av);

/**
 * multiboot info header parser
 */
void check_multiboot(Multiboot_info_t* header, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("Wrong multiboot magic number, multiboot header is not present");
	}
	if (!MULTIBOOT_TEST_FLAG(header, MULTIBOOT_INFO_MEMORY)) {
		PANIC("Multiboot memmory flags is not present, manual memory detection "
		      "is not available");
	}
	if (!(header->flags & MULTIBOOT_INFO_MMAP)) {
		PANIC("Multiboot memory map is not present, manual memory detection is "
		      "not available");
	}
	printf("Lower memory region size: %dKiB\n", header->mem_lower);
	printf("Upper memory region size: %dKiB\n", header->mem_upper);
}

/**
 * @brief Architecture entry point called from the init code
 */

void I386_entry_point(Multiboot_info_t* header, uint32_t magic) {
	Init_gdt();
	Init_idt();
	asm volatile("sti");
	Cpu_register_interrupt_handler(14, &boot_page_fault_handler);
	monitor_driver_init();
	printf("Booting init code i386_GENERIC\n");
	check_multiboot(header, magic);
	Init_memory(header->mmap_addr, header->mmap_length);
	Init_apic();
	main(0, NULL);
}
