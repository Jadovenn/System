/**
 * gdt_init.c - Global Descriptor Table Init
 * System sourcies under license MIT
 */

#include <cpu/gdt.h>

extern void	gdt_flush(uint32_t);

gdt_entry_t	gdt_entries[5];
gdt_ptr_t	gdt_ptr;

static void	__gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt_entries[num].base_low = (base & 0xffff);
	gdt_entries[num].base_middle = (base >> 16) & 0xff;
	gdt_entries[num].base_hight = (base >> 24) & 0xff;

	gdt_entries[num].limit_low = (limit & 0xffff);
	gdt_entries[num].granularity = (limit >> 16) & 0x0f;

	gdt_entries[num].granularity |= gran & 0xf0;
	gdt_entries[num].access = access;
}

void	install_gdt() {
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base = (uint32_t)&gdt_entries;

	__gdt_set_gate(NULL_SEGMENT, 0, 0, 0, 0);
	__gdt_set_gate(CODE_SEGMENT, 0, 0xffffffff ,0x9a, 0xcf);
	__gdt_set_gate(DATA_SEGMENT, 0, 0xffffffff ,0x92, 0xcf);
	__gdt_set_gate(USER_MODE_CODE_SEGMENT, 0, 0xffffffff ,0xfa, 0xcf);
	__gdt_set_gate(USER_MODE_DATA_SEGMENT, 0, 0xffffffff ,0xf2, 0xcf);
	gdt_flush((uint32_t)&gdt_ptr);
}

