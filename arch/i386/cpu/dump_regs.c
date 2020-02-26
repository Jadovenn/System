/**
 * dump_regs.c - dump registers to the monitor
 * System sources under license MIT
 */

#include <kernel/stdio.h>
#include <cpu/isr.h>

void	dump_regs_from_interrupt(registers_t regs) {
	printk(" eax: %#x   ebx: %#x   ecx: %#x\n",
			regs.eax,
			regs.ebx,
			regs.ecx);
	printk(" edx: %#x   edi: %#x   esi: %#x\n",
			regs.edx,
			regs.edi,
			regs.esi);
	printk(" ebp: %#x   esp: %#x   eip: %#x\n",
			regs.ebp,
			regs.esp,
			regs.eip);
	printk("  cs: %#x    ds: %#x    ss: %#x\n",
			regs.cs,
			regs.ds,
			regs.ss);
	printk(" eflags: %#x\n", regs.eflags);
}

