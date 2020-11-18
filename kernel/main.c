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
	}
	return 0;
}
