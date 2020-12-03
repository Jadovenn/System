/**
 * apic.c - Advanced Programmable Interrupt Controller
 * System sources under license MIT
 */

#include "cpu/apic/apic.h"

#include <hal/memory.h>
#include <kernel/panic.h>
#include <kernel/stdio.h>

#include <arch/ports.h>
#include <cpu/pic.h>

#include "cpu/registers/msr.h"

#include <cpuid/cpuid.h>

void Init_apic() {
	if (!cpuid_is_supported()) {
		PANIC("CPUID not available. You may want to provide a specific apic "
		      "detection for your cpu.");
	}
	Cpuid_capabilities_t cap = cpuid_get_capabilities();
	if (!cap.APIC) {
		PANIC("APIC not available. System only support CPUs with apic.");
	}
	if (!cap.MSR) {
		PANIC("Model Specific registers not present.");
	}

	uint32_t hi, lo;
	Cpu_read_msr(IA32_APIC_BASE_MSR, &hi, &lo);

	uintptr_t msrBase = (lo & 0xfffff000);
	void* msr = Hal_mmap(msrBase, 1, map_ANONYMOUS | map_DATA | map_READONLY);
	if (msr == MAP_FAILED) {
		PANIC("MSR base mapping failed :(\n");
	}

  port_write_byte(0xF0, port_read_byte(0xF0) | 0x100);
  Pic_disable();
	printf("APIC is ready.\n");
}
