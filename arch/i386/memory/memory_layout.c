/**
 * memlayout.c - Kernel virtual memory area
 * System sources under license MIT
 */

#include "memory/virtual.h"

Virtual_memory_area_t G_Kernel_vma[] = {
		{0x00000000, 0xBFFFFFFF, vmt_USERSPACE | vmt_DAEMON},
		{0xC0000000, 0xC00FFFFF, vmt_BIOS},
		{0xC0100000, 0xCFFFFFFF, vmt_KERNEL_IMAGE},
		{0xD0000000, 0xDFFFFFFF, vmt_HEAP},
		{0xE0000000, 0xEFFFFFFF, vmt_DATA},
		{0xF0000000, 0xFFFFFFFF, vmt_DATA | vmt_DAEMON},
		{0x0, 0x0, vmt_END}};
