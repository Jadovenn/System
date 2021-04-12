/**
 * multiboot.h - Multiboot specification types
 * System sources under license MIT
 */

#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_MAGIC            0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

#define MULTIBOOT_INFO_MEMORY 0x001
#define MULTIBOOT_INFO_MMAP   0x020
#define MULTIBOOT_INFO_DRIVES 0x07

#define MULTIBOOT_TEST_FLAG(header, flag) (header->flags & flag)

struct Multiboot_info
{
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
	uint32_t mmap_length;
	uint32_t mmap_addr;
	uint32_t drives_length;
	uint32_t drives_addr;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
} __attribute__((packed));

typedef struct Multiboot_info Multiboot_info_t;

struct Multiboot_mmap_region
{
	uint32_t size;
	uint64_t addr;
	uint64_t len;
	uint32_t type;
} __attribute__((packed));

typedef struct Multiboot_mmap_region Multiboot_mmap_region_t;

typedef enum Multiboot_memory_type
{
	mmt_AVAILABLE        = 1,
	mmt_RESERVED         = 2,
	mmt_ACPI_RECLAIMABLE = 3,
	mmt_NVS              = 4,
	mmt_BAD_RAM          = 5
} Multiboot_memory_type_e;

#define MULTIBOOT_MEMORY_AVAILABLE        1
#define MULTIBOOT_MEMORY_RESERVED         2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS              4
#define MULTIBOOT_MEMORY_BADRAM           5

struct Multiboot_drive_region_s
{
	uint32_t  size;
	uint8_t   drive_number;
	uint8_t   drive_mode;
	uint16_t  drive_cylinders;
	uint8_t   drive_head;
	uint8_t   drive_sectors;
	uint16_t* drive_ports;
} __attribute__((packed));

typedef struct Multiboot_drive_region_s Multiboot_drive_region_t;

#endif // MULTIBOOT_H
