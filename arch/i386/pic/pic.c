/**
 * pic.c - Programable Interrupt Controller 8259 API
 * System sources under license MIT
 */

#include <cpu/isr.h>
#include <cpu/pic.h>

#include <arch/io.h>
#include <arch/ports.h>

void	PIC_disable() {
	//asm("mov %al, 0xff");
	//asm("outb 0xa1, %al");
	//asm("out 0x21, %al");
}

void	PIC_send_EOI(unsigned char irq) {
	if (irq >= 8) {
		port_write_byte(PIC2_COMMAND, PIC_EOI);
	}
	port_write_byte(PIC1_COMMAND, PIC_EOI);
}

