/**
 * main.c - kernel common main
 * System sources under license
 */

<<<<<<< HEAD
#include <kernel/stdio.h>

#include <cpuid/cpuid.h>
#include <system.h>

int main(int ac, char** av) {
	(void)ac;
	(void)av;
	printf("System - ExoKernel version %s\n", VERSION);
	int result = cpuid_is_supported();
	if (result) {
		Cpuid_processor_vendor_t vendor = cpuid_get_vendor();
		printf("(vendor)   %s\n", cpuid_get_vendor_string(vendor));
		const char* model = cpuid_get_model_string();
		if (model) {
			printf("(CPU)      %s\n", model);
		}
	}
	return 0;
}
=======
int	main(int ac, char **av) {
	(void)ac;
	(void)av;
	return 0;
}

>>>>>>> ddc1364... Initial commit: System
