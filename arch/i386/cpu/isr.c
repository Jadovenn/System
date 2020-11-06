/**
 * isr.c - Interruptions request handler API
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <arch/ports.h>
#include <kernel/compiler.h>
#include <kernel/panic.h>
#include <kernel/stdio.h>

#include <cpu/cr.h>
#include <cpu/isr.h>

isr_t interrupt_handlers_map[256];

int Cpu_register_interrupt_handler(uint8_t index, isr_t handler) {
	interrupt_handlers_map[index] = handler;
	return EXIT_SUCCESS;
}

void isr_handler(Cpu_registers_t regs) {
	if (interrupt_handlers_map[regs.interr_nb] != NULL) {
		isr_t handler = interrupt_handlers_map[regs.interr_nb];
		handler(regs);
	} else {
		uint32_t cr2 = Cpu_read_cr2();
		printf("cr2: %#x\n", cr2);
		Cpu_dump_registers(regs);
		PANIC("Unmanaged fault: %d", regs.interr_nb);
	}
}

void irq_handler(Cpu_registers_t regs) {
	if (regs.interr_nb >= 40) {
		port_write_byte(0xa0, 0x20);
	}
	port_write_byte(0x20, 0x20);
	if (interrupt_handlers_map[regs.interr_nb] != NULL) {
		isr_t handler = interrupt_handlers_map[regs.interr_nb];
		handler(regs);
	}
}
