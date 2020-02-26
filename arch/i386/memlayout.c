/**
 * memlayout.c - Kernel virtual memory aera 
 * System sources under license MIT
 */

#include <arch/memlayout.h>
#include <arch/paging.h>
#include <api/mm.h>

/**
 * Kernel Virtual Memory Area
 */
vma_struct_t	_kernel_vma[] = {
	{ 0x00000000, 0xBFFFFFFF, 0x3, 0},
	{ 0xC0000000, 0xC00FFFFF, 0x1, 1},
	{ 0xC0100000, 0xCFFFFFFF, 0x3, 3},
	{ 0xD0000000, 0xDFFFFFFF, 0x3, 4},
	{ 0xE0000000, 0xEFFFFFFF, 0x3, 5},
	{ 0xF0000000, 0xFFFFFFFF, 0x3, 6},
	{ 0x0, 0x0, 0x0, 0x0}
};

/**
 * Kernel Physical Memory Region
 * start and end addr are included in the range
 * code type after 7 are free to use
 * Please list here special reservation:
 * 	-- MULTIBOOT --
 * 	1: available RAM
 * 	2: reserved aera
 * 	3: ACPI aera
 * 	4: reserved aera
 * 	5: defective RAM
 * 	-- System --
 * 	6: BIOS aera
 * 	7: kernel aera
 * 	8: low memory
 * 	9: VGA Buffer
 */
pm_region_t	_kernel_pm_region[] = {
	{ 0x000B8000, 0x000B8FFF, 9 },
	{ VIRTUAL_ADDR_TO_PHYSICAL((uintptr_t)&_kernel_start),
		VIRTUAL_ADDR_TO_PHYSICAL((uintptr_t)&_end), 7 },
	{ VIRTUAL_ADDR_TO_PHYSICAL((uintptr_t)&_end),
		0x00FFFFFF, 8},
	{ 0x0, 0x0, 0x0}
};

