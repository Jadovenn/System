/**
 *	System Kernel main include
 */

#ifndef SYSTEM_H_
# define SYSTEM_H_

#define VERSION "0.0.1"

#include <kernel/stdio.h>

#define NULL ((void*)0)

#define SUCCESS	0
#define ERROR	1

#define TRUE	1
#define FALSE	0

#define PANIC(format, ...)	do {\
	printk("Sytem kernel PANIC in %s at %s:\n", __FILE__, __LINE__);\
	printk(format __VA_OPT__(,) __VA_ARGS__);\
	while(1);\
} while (0)

#endif // SYSTEM_H_

