/**
 * system.h - kernel general purpose
 * System sources under license MIT
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
	printk("Sytem panic in %s at line %d:\n", __FILE__, __LINE__);\
	printk(format __VA_OPT__(,) __VA_ARGS__);\
	printk("\n!!!!!! KERNEL PANIC !!!!!!\n");\
	while(1);\
} while (0)

#endif // SYSTEM_H_

