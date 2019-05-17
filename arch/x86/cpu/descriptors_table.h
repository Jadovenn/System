/**
 * Interrupt Descriptor Table
 */

#ifndef IDT_H_
# define IDT_H_

#include <kernel/types.h>

typedef struct gdt_entry_struct {
	uint16_t	limit_low;	// lower 16bits of the limit
	uint16_t	base_low;	// lower 16bits of the base
	uint8_t		base_middle;	// next 8bits of the base
	uint8_t		access;		// Access flags, determine in which ring this segment can be used
	uint8_t		granularity;
	uint8_t		base_hight;	// last 8bits of the base
} __attribute__((packed))	gdt_entry_t;	

typedef struct gdt_pointer_struct {
	uint16_t	limit;	// upper 16 bits of all sector limits
	uint32_t	base;	// address of the first gdt_entry_t
} __attribute__((packed)) gdt_ptr_t;

enum gdt_segment {
	NULL_SEGMENT = 0,
	CODE_SEGMENT = 1,
	DATA_SEGMENT = 2,
	USER_MODE_CODE_SEGMENT = 3,
	USER_MODE_DATA_SEGMENT = 4,
};

typedef struct idt_entry_struct {
	uint16_t	base_lo;	// lowest 16 bit of the address to jump
	uint16_t	sel;		// kernel segment selector
	uint8_t		always0;	// always 0
	uint8_t		flags;		// flags see all documentation
	uint16_t	base_hi;	// upper 16bits of the addr to jump
} __attribute__((packed))	idt_entry_t;

typedef struct	idt_pointer_struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed))	idt_ptr_t;

extern void	isr0();
extern void	isr1();
extern void	isr2();
extern void	isr3();
extern void	isr4();
extern void	isr5();
extern void	isr6();
extern void	isr7();
extern void	isr8();
extern void	isr9();
extern void	isr10();
extern void	isr11();
extern void	isr12();
extern void	isr13();
extern void	isr14();
extern void	isr15();
extern void	isr16();
extern void	isr17();
extern void	isr18();
extern void	isr19();
extern void	isr20();
extern void	isr21();
extern void	isr22();
extern void	isr23();
extern void	isr24();
extern void	isr25();
extern void	isr26();
extern void	isr27();
extern void	isr28();
extern void	isr29();
extern void	isr30();
extern void	isr31();
extern void	irq0();
extern void	irq1();
extern void	irq2();
extern void	irq3();
extern void	irq4();
extern void	irq5();
extern void	irq6();
extern void	irq7();
extern void	irq8();
extern void	irq9();
extern void	irq10();
extern void	irq11();
extern void	irq12();
extern void	irq13();
extern void	irq14();
extern void	irq15();


#endif // IDT_H_

