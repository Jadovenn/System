/**
 * panic.h - PANIC
 * System sources under license MIT
 */

#ifndef PANIC_H_
#define PANIC_H_

#include <stdlib.h>

#include <kernel/stdio.h>

#define PANIC(format, ...)                                                     \
	do {                                                                         \
		printk(">>>>>>>>> PANIC <<<<<<<<<<\n");                                    \
		printk("System panic in %s at line %d\n", __FILE__, __LINE__);             \
		printk(format __VA_OPT__(, ) __VA_ARGS__);                                 \
		printk("\n!!!!!! KERNEL PANIC !!!!!!\n");                                  \
		abort();                                                                   \
	} while (0);

#endif // PANIC_H_
