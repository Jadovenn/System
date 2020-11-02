/**
 * gdt.h - Global Descriptor Table
 * System sources under license MIT
 */

#ifndef GDT_H_
#define GDT_H_

#include <stdint.h>

typedef struct Gdt_entry_struct {
	uint16_t limit_low;   // lower 16bits of the limit
	uint16_t base_low;    // lower 16bits of the base
	uint8_t  base_middle; // next 8bits of the base
	uint8_t
			access; // Access flags, determine in which ring this segment can be used
	uint8_t granularity;
	uint8_t base_hight; // last 8bits of the base
} __attribute__((packed)) gdt_entry_t;

typedef struct Gdt_pointer_struct {
	uint16_t limit; // upper 16 bits of all sector limits
	uint32_t base;  // address of the first gdt_entry_t
} __attribute__((packed)) gdt_ptr_t;

enum Gdt_segment {
	NULL_SEGMENT           = 0,
	CODE_SEGMENT           = 1,
	DATA_SEGMENT           = 2,
	USER_MODE_CODE_SEGMENT = 3,
	USER_MODE_DATA_SEGMENT = 4,
};

void Init_gdt();

#endif // GDT_H_
