/**
 * main.c - kernel common main
 * System sources under license
 */

#include <kernel/stdio.h>

#include <cpuid/cpuid.h>
#include <system.h>

int main(int ac, char** av) {
	(void)ac;
	(void)av;
	printf("System - ExoKernel version %s\n", VERSION);
	printf("-----------------------------------------------\n");
	printf("Main function reached. Arch init passed.\n");
	printf("System will now halt.\n");
	printf("-----------------------------------------------\n");
	int result = cpuid_is_supported();
	if (result) {
		Cpuid_processor_vendor_t vendor = cpuid_get_vendor();
		printf("cpu vendor: %s\n", cpuid_get_vendor_string(vendor));
		const char* model = cpuid_get_model_string();
		if (model) {
			printf("cpu model:  %s\n", model);
		}
		Cpuid_capabilities_t cpuidCap = cpuid_get_capabilities();
		if (cpuidCap.SSE) {
			printf("cpuid sse feature present\n");
		}
		if (cpuidCap.PAE) {
			printf("cpuid pea feature present");
		}
	}
	return 0;
}
