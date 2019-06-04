/**
 * Timer
 */

#include <system.h>
#include <isr.h>
#include <kernel/types.h>
#include <kernel/io.h>
#include <kernel/stdio.h>
#include <kernel/ports.h>
#include <drivers/monitor.h>

uint32_t	tick;

static void __timer_callback(registers_t regs) {
	tick++;
	printk("timer tick %d\n", tick);
	if (tick == 15) {
		register_interrupt_handler(IRQ_0, NULL);
	}
}

void	init_timer(uint32_t frequency) {
	int ret = register_interrupt_handler(IRQ_0, &__timer_callback);
	if (ret == ERROR)
		printk("register error\n");
	uint32_t divisor = 1193180 / frequency;
	port_write_byte(0x43, 0x36);
	uint8_t	low = (uint8_t)(divisor & 0xff);
	uint8_t hight = (uint8_t)((divisor >> 8) & 0xff);
	port_write_byte(0x40, low);
	port_write_byte(0x40, hight);
}

