/**
 * main.c - kernel common main
 * System sources under license
 */

#include <kernel/stdio.h>

#include <system.h>

int main(int ac, char** av) {
	(void)ac;
	(void)av;
	printf("System - ExoKernel version %s\n", VERSION);
	printf("-----------------------------------------------\n");
	printf("Main function reached. Arch init passed.\n");
	printf("System will now halt.\n");
	printf("-----------------------------------------------\n");
	return 0;
}
