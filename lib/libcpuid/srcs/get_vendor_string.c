/**
 * get_vendor_string.c - match vendor id to string
 * System sources under license MIT
 */

#include <stddef.h>

#include <cpuid/cpuid.h>

const char* vendor_strings[cpu_vendor_COUNT] = {
		"unknown", "AMD", "Intel",        "Transmeta", "Cyrix",  "NexGen",
		"UMC",     "SiS", "NationalSemiconductor",          "Rise",      "Vortex", "VIA",
		"VMware",  "Xen", "Microsoft_hv", "Parallels", "Qemu"
};

const char* cpuid_get_vendor_string(Cpuid_processor_vendor_t aVendorCode) {
	if (aVendorCode < -1 || aVendorCode > cpu_vendor_COUNT - 2) {
		return NULL;
	}
	return vendor_strings[aVendorCode + 1];
}
