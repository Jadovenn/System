/**
 * dump_regs.c - dump registers to the monitor
 * System sources under license MIT
 */

#include <kernel/stdio.h>

#include <cpu/isr.h>

void Cpu_dump_registers(Cpu_registers_t regs)
{
	printf(" eax: %#x   ebx: %#x   ecx: %#x\n", regs.eax, regs.ebx, regs.ecx);
	printf(" edx: %#x   edi: %#x   esi: %#x\n", regs.edx, regs.edi, regs.esi);
	printf(" ebp: %#x   esp: %#x   eip: %#x\n", regs.ebp, regs.esp, regs.eip);
	printf("  cs: %#x    ds: %#x    ss: %#x\n", regs.cs, regs.ds, regs.ss);
	printf(" eflags: %#x\n", regs.eflags);
}
