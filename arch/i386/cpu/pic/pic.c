/**
 * pic.c - Programmable Interrupt Controller 8259 API
 * System sources under license MIT
 */

#include <arch/io.h>
#include <arch/ports.h>

#include <cpu/isr.h>
#include <cpu/pic.h>

/**
void Pic_disable() {
	__asm__("mov  %al, 0xff");
	__asm__("out  0xa1, %al");
	__asm__("out  0x21, %al");
}
 */

void Pic_send_EOI(unsigned char irq) {
	if (irq >= 8) {
		port_write_byte(PIC2_COMMAND, PIC_EOI);
	}
	port_write_byte(PIC1_COMMAND, PIC_EOI);
}
