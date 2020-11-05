/**
 * main.c - kernel common main
 * System sources under license
 */

#include <kernel/stdio.h>

int main(int ac, char** av) {
	(void)ac;
	(void)av;
	printk("System - ExoKernel version %s\n", VERSION);
	printk("-----------------------------------------------\n");
	printk("Main function reached. Arch init passed.\n");
	printk("System will now halt.\n");
	printk("-----------------------------------------------\n");
	return 0;
}
