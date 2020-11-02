/**
 * virtual.h - Virtual Memory Management
 * System sources under license MIT
 */

#ifndef I386_VIRTUAL_H
#define I386_VIRTUAL_H

#include <stdint.h>

#include <hal/memory.h>

/********************************
 **     Structure and Enum     **
 ********************************/

typedef enum Virtual_memory_type {
	vmt_USERSPACE    = 0x00000001,
	vmt_BIOS         = 0x00000010,
	vmt_RESERVED     = 0x00000100,
	vmt_KERNEL_IMAGE = 0x00001000,
	vmt_LOW_MEMORY   = 0x00010000,
	vmt_HEAP         = 0x00100000,
	vmt_DATA         = 0x01000000,
	vmt_DAEMON       = 0x10000000,
	vmt_END
} Virtual_memory_type_e;

typedef struct Virtual_memory_area {
	uintptr_t             start;
	uintptr_t             end;
	Virtual_memory_type_e type;
} Virtual_memory_area_t;

/********************************
 **    Globals declaration     **
 ********************************/

extern uint32_t              G_Page_directory;
extern uint32_t              G_Page_table_entries;
extern Virtual_memory_area_t G_Kernel_vma[];

/********************************
 **     Public functions       **
 ********************************/

void Init_virtual_memory();

#endif // I386_VIRTUAL_H
