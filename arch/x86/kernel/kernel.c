/**
 * kernel.c - x86 kernel entry point
 * System sources under license MIT
 */

#include <stdint.h>
#include <kernel/panic.h>
#include <kernel/multiboot.h>
#include <kernel/heap.h>
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

void	memory_alloc_test() {
	int block_size = sizeof(uint32_t);
	int block_total_nb = 4096 / 4;
	int block_nb = 0;
	uint32_t *ptr = NULL;
	uint32_t *it = NULL;
	it = kmalloc(block_size);
	ptr = it;
	block_nb += 1;
	for (; block_nb < block_total_nb; block_nb += 1) {
		printk("alloc number: %d\n", block_nb);
		uint32_t *block = kmalloc(block_size);
		if (!it)
			break;
		*it = (uint32_t)block;
		it = block;
	}
	printk("nb block allocated: %d\n", block_nb);
	while (ptr != it) {
		uint32_t *next = (uint32_t*)*ptr;
		kfree(ptr);
		ptr = next;
	}
	kfree(ptr);
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
	//init_timer(50);
	while(1);
}

