/**
 * isr.c - Interruptions request handler API
 * System sources under license MIT
 */

#include <system.h>
#include <kernel/compiler.h>
#include <kernel/ports.h>
#include <kernel/stdio.h>
#include <cpu/isr.h>

isr_t	interrupt_handlers_map[256];

int	register_interrupt_handler(uint8_t index, isr_t handler) {
	interrupt_handlers_map[index] = handler;
	return SUCCESS;
}

void	isr_handler(registers_t regs) {
	printk("Fault: %d\n", regs.interr_nb);
	if (interrupt_handlers_map[regs.interr_nb] != 0) {
		isr_t handler = interrupt_handlers_map[regs.interr_nb];
		handler(regs);
	}
}

void	irq_handler(registers_t regs) {
	if (regs.interr_nb >= 40) {
		port_write_byte(0xa0, 0x20);
	}
	port_write_byte(0x20, 0x20);
	if (interrupt_handlers_map[regs.interr_nb] != 0) {
		isr_t handler = interrupt_handlers_map[regs.interr_nb];
		handler(regs);
	}
}

