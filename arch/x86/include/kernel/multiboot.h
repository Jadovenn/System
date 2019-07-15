/**
 * multiboot.h - multiboot header
 * System under license MIT
 */

#ifndef MULTIBOOT_H_
# define MULTIBOOT_H_

#include <kernel/types.h>

#define MULTIBOOT_MAGIC			0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

#define MULTIBOOT_INFO_MEMORY		0x001
#define MULTIBOOT_INFO_MMAP		0x020

struct multiboot_info_t {
	uint32_t	flags;
	uint32_t	mem_lower;
	uint32_t	mem_upper;
	uint32_t	boot_device;
	uint32_t	cmdline;
	uint32_t	mods_count;
	uint32_t	mods_addr;
	uint32_t	num;
	uint32_t	size;
	uint32_t	addr;
	uint32_t	shndx;
	uint32_t	mmap_length;
	uint32_t	mmap_addr;
	uint32_t	drives_length;
	uint32_t	drives_addr;
	uint32_t	config_table;
	uint32_t	boot_loader_name;
	uint32_t	apm_table;
	uint32_t	vbe_control_info;
	uint32_t	vbe_mode_info;
	uint32_t	vbe_mode;
	uint32_t	vbe_interface_seg;
	uint32_t	vbe_interface_off;
	uint32_t	vbe_interface_len;
} __attribute__((packed));

typedef struct multiboot_info_t	multiboot_info;

#endif // MULTIBOOT_H_

