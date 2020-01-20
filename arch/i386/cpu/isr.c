/**
 * isr.c - Interruptions request handler API
 * System sources under license MIT
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <kernel/compiler.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>

#include "cpu/isr.h"
#include "arch/ports.h"

isr_t	interrupt_handlers_map[256];

int	register_interrupt_handler(uint8_t index, isr_t handler) {
	interrupt_handlers_map[index] = handler;
	return EXIT_SUCCESS;
}

void	isr_handler(registers_t regs) {
	if (interrupt_handlers_map[regs.interr_nb] != NULL) {
		isr_t handler = interrupt_handlers_map[regs.interr_nb];
		handler(regs);
	}
	else {
		PANIC("Unmanaged fault: %d", regs.interr_nb);
	}
}

void	irq_handler(registers_t regs) {
	if (regs.interr_nb >= 40) {
		port_write_byte(0xa0, 0x20);
	}
	port_write_byte(0x20, 0x20);
	if (interrupt_handlers_map[regs.interr_nb] != NULL) {
		isr_t handler = interrupt_handlers_map[regs.interr_nb];
		handler(regs);
	}
}

