/**
 * memlayout.h - Physical Memory Layout
 * System under license MIT
 */

#ifndef MEMLAYOUT_H_
# define MEMLAYOUT_H_

#include <kernel/types.h>

#define VIRATUAL_KERNEL_SPACE_ADDR 0xC0000000

#define VIRTUAL_PTR_TO_PHYSICAL(x)	(uint32_t*)(x - VIRTUAL_KERNEL_SPACE_ADDR)
#define PHYSICAL_PTR_TO_VIRTUAL(x)	(uint32_t*)(x + VIRTUAL_KERNEL_SPACE_ADDR)

extern uint32_t	*_kernel_start;
extern uint32_t	*code;
extern uint32_t	*ecode;
extern uint32_t	*edata;
extern uint32_t	*bss;
extern uint32_t	*end;

extern uint32_t	boot_page_directory;
extern uint32_t	boot_page_table;

#endif // MEMLAYOUT_H_

