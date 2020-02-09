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
vma_t	kernel_vma[] = {
	{ 0x00000000, 0xBFFFFFFF, 0x3, "userspace"},
	{ 0xC0000000, 0xC00FFFFF, 0x1, "bios"},
	{ 0xC0100000, 0xCFFFFFFF, 0x3, "kernel"},
	{ 0xD0000000, 0xDFFFFFFF, 0x3, "heap"},
	{ 0xE0000000, 0xEFFFFFFF, 0x3, "devices"},
	{ 0xF0000000, 0xFFFFFFFF, 0x3, "data"},
	{0x0, 0x0, 0x0, NULL}
};

/**
 * Kernel Physical Memory Area
 * TODO:
 * 	in vitual_memory/init.c add dma entry in this
 * 	table after the dma area is determinated
 */
pma_t	kernel_pmr[] = {
	{0x00000000, 0x000FFFFF, "bios"},
	{(uintptr_t)&_kernel_start, (uintptr_t)&_end, "kernel"},
	{0x0, 0x0, "dma"},
	{0x0, 0x0, NULL}
};

