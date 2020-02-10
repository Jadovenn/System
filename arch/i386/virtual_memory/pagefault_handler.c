/**
 * pagefault_handler.c - Handle PageFault interruption
 * System sources under license MIT
 */

#include <stdint.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>

#include <cpu/cr.h>
#include <cpu/isr.h>

static const char *PG_FAULT_USER_ALL = "::: Page Fault caused by user are not managed yet";
static const char *PG_FAULT_KERNEL_000 = "::: Kernel tried to read a non present page";
static const char *PG_FAULT_KERNEL_001 = "::: Kernel tried to read a page and caused a protection fault";
static const char *PG_FAULT_KERNEL_010 = "::: Kernel tried to write to a non-present page entry";
static const char *PG_FAULT_KERNEL_011 = "::: Kernel tried to write a page and caused a protection fault";
static const char *PG_FAULT_DEFAULT = "::: Unexpected Page Fault Interrution";

void	boot_page_fault_handler(registers_t regs) {
	uint32_t cr2 = read_cr2();
	uint32_t mask = regs.err_code & 0x00000007;
	if (mask & 0x4) {
		PANIC(PG_FAULT_USER_ALL);
	}
	printk("\n:::: INTERRUPT %d ::::\n", regs.interr_nb);
	printk("Page Fault at: %#x\n", cr2);
	dump_regs_from_interrupt(regs);
	if (!mask) {
		PANIC(PG_FAULT_KERNEL_000);
	}
	if (mask == 1) {
		PANIC(PG_FAULT_KERNEL_001);
	}
	if (mask == 2) {
		PANIC(PG_FAULT_KERNEL_010);
	}
	if (mask == 3) {
		PANIC(PG_FAULT_KERNEL_011);
	}
	PANIC(PG_FAULT_DEFAULT);
}

