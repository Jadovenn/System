/**
 * isr handler
 */

#include "isr.h"
#include <drivers/monitor.h>

void	isr_handler(registers_t regs) {
	printk("Interruption catch ");
	char c = regs.interr_nb + 48;
	printk_char(c);
	printk_char('\n');
}

