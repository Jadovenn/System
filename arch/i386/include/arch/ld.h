/**
 * ld.h - linker script and helper
 * System under license MIT
 */

#ifndef LD_H_
#define LD_H_

#include <stdint.h>

/********************************
 ** Address translation macros **
 ********************************/

#define VIRTUAL_KERNEL_SPACE_ADDR 0xC0000000

#define VIRTUAL_PTR_TO_PHYSICAL(x)                                             \
	(uint32_t*)(((uint32_t)x) - VIRTUAL_KERNEL_SPACE_ADDR)
#define PHYSICAL_PTR_TO_VIRTUAL(x)                                             \
	(uint32_t*)(((uint32_t)x) + VIRTUAL_KERNEL_SPACE_ADDR)

#define VIRTUAL_ADDR_TO_PHYSICAL(x)                                            \
	(uint32_t)(((uint32_t)x) - VIRTUAL_KERNEL_SPACE_ADDR)
#define PHYSICAL_ADDR_TO_VIRTUAL(x)                                            \
	(uint32_t)(((uint32_t)x) + VIRTUAL_KERNEL_SPACE_ADDR)

/*************************************
 ** ld script variables declaration **
 *************************************/

extern uint32_t G_Kernel_space;
extern uint32_t G_Start_kernel;
extern uint32_t G_Start_code;
extern uint32_t G_End_code;
extern uint32_t G_Start_rodata;
extern uint32_t G_End_rodata;
extern uint32_t G_Start_data;
extern uint32_t G_End_data;
extern uint32_t G_Start_bss;
extern uint32_t G_End_bss;
extern uint32_t G_End_kernel;

/*************************************
 ** init code variables declaration **
 *************************************/

extern uint32_t boot_page_directory;
extern uint32_t boot_pte_database;
extern uint32_t boot_page_table;

#endif // LD_H_
