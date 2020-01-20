/**
 * dump_regs.c - dump registers to the monitor
 * System sources under license MIT
 */

#include <kernel/stdio.h>

#include "cpu/isr.h"

void	dump_regs_from_interrupt(registers_t regs) {
	printk("Interrupt nb: %d\n", regs.interr_nb);
	printk(" eax: %#x\n", regs.eax);
	printk(" ebx: %#x\n", regs.ebx);
	printk(" ecx: %#x\n", regs.ecx);
	printk(" edx: %#x\n", regs.edx);
	printk(" edi: %#x\n", regs.edi);
	printk(" esi: %#x\n", regs.esi);
	printk(" ebp: %#x\n", regs.ebp);
	printk(" esp: %#x\n", regs.esp);
	printk(" eip: %#x\n", regs.eip);
	printk("  cs: %#x\n", regs.cs);
	printk("  ds: %#x\n", regs.ds);
	printk("  ss: %#x\n", regs.ss);
	printk(" eflags: %#x\n", regs.eflags);
}

