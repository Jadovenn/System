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

