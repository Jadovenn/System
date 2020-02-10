/**
 * memlayout.c - Kernel virtual memory aera 
 * System sources under license MIT
 */

#include <stddef.h>
#include <arch/memlayout.h>
#include <arch/paging.h>
#include <api/mm.h>

/**
 * Kernel Virtual Memory Area
 */
vma_struct_t	kernel_vma[] = {
	{ 0x00000000, 0xBFFFFFFF, 0x3, 2},
	{ 0xC0000000, 0xC00FFFFF, 0x1, 0},
	{ 0xC0100000, 0xCFFFFFFF, 0x3, 2},
	{ 0xD0000000, 0xDFFFFFFF, 0x3, 4},
	{ 0xE0000000, 0xEFFFFFFF, 0x3, 5},
	{ 0xF0000000, 0xFFFFFFFF, 0x3, 6},
	{ 0x0, 0x0, 0x0, 0x0}
};

/**
 * Kernel Physical Memory Area
 * TODO:
 * 	in vitual_memory/init.c add dma entry in this
 * 	table after the dma area is determinated
 */
pma_t	kernel_pmr[] = {
	{ 0x00000000, 0x000FFFFF, "bios"},
	{ VIRTUAL_ADDR_TO_PHYSICAL((uintptr_t)&_kernel_start),
		VIRTUAL_ADDR_TO_PHYSICAL((uintptr_t)&_end), "kernel"},
	{ 0x0, 0x0, "dma"},
	{ 0x0, 0x0, NULL}
};

