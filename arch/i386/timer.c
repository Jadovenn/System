/**
 * timer.c - Timer functionality
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdlib.h>

#include <kernel/stdio.h>

#include "arch/io.h"
#include "arch/ports.h"
#include "cpu/isr.h"

uint32_t tick = 0;

static void __timer_callback(registers_t regs) {
	(void)regs;
	tick++;
	printk("timer tick %d\n", tick);
	if (tick == 15) {
		register_interrupt_handler(IRQ_0, NULL);
	}
}

/**
 * @brief register PIT IRQ
 * @details config register value send to port 0x43:
 * bit [0] = 0: binary counter 16bits
 * bit [1-3] = x11: square wave generator
 * bit [4-5] = 11: counter bits 0-7 first, then 8-15
 * bit [5-7] = 0: counter 0 select
 * @param frequency - freq for IRQ to be trigger by the PIT
 */
void init_timer(uint32_t frequency) {
	int ret = register_interrupt_handler(IRQ_0, &__timer_callback);
	if (ret == EXIT_FAILURE)
		printk("register error\n");
	uint32_t divisor = 1193180 / frequency;
	port_write_byte(0x43, 0b00110110);
	uint8_t low   = (uint8_t)(divisor & 0xff);
	uint8_t hight = (uint8_t)((divisor >> 8) & 0xff);
	port_write_byte(0x40, low);
	port_write_byte(0x40, hight);
}
