/**
 * init.h - init early stage hooks
 * System sources under license MIT
 */

/**
 * Note
 * sub fonction to init intern stuff
 * put here extra func if necessary
 * only call by k_init, they must be called in their declaration order
 */

#ifndef INIT_H_
# define INIT_H_

#include <kernel/multiboot.h>

//
//	CPU Init
//
void	gdt_init();
void	idt_init();
void	boot_paging_init();

//
//	Drivers Init
//
int	monitor_driver_init();

//
//	Pre Kernel Init
//
void	kernel_paging_init(multiboot_info *header);
void	physical_memory_init(multiboot_info *header);

#endif // INIT_H_

