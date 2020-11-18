/**
 * cpuid.h - cpuid main header
 * System sources under license MIT
 */

#ifndef _CPUID_H
#define _CPUID_H

#include <stdbool.h>

typedef enum Cpuid_processor_vendor_e {
	cpu_vendor_UNKNOW       = -1,
	cpu_vendor_AMD          = 0,
	cpu_vendor_INTEL        = 1,
	cpu_vendor_TRANSMETA    = 2,
	cpu_vendor_CYRIX        = 3,
	cpu_vendor_CENTAUR      = 4,
	cpu_vendor_NEXGEN       = 5,
	cpu_vendor_UMC          = 6,
	cpu_vendor_SIS          = 7,
	cpu_vendor_NSC          = 8,
	cpu_vendor_RISE         = 9,
	cpu_vendor_VORTEX       = 10,
	cpu_vendor_VIA          = 11,
	cpu_vendor_VMWARE       = 12,
	cpu_vendor_XEN          = 13,
	cpu_vendor_MICROSOFT_HV = 14,
	cpu_vendor_PARALLEL     = 15,
	cpu_vendor_QEMU = 16,
	cpu_vendor_END,
	cpu_vendor_COUNT = cpu_vendor_END + 1,
} Cpuid_processor_vendor_t;

int cpuid_is_supported();
int cpuid_max();

Cpuid_processor_vendor_t cpuid_get_vendor();
const char*              cpuid_get_vendor_string(Cpuid_processor_vendor_t);
const char*              cpuid_get_model_string();

#endif // _CPUID_H
