/**
 * monitor_init.c - clear monitor and print kernel version
 * System sources under license MIT
 */

#include <system.h>
#include <drivers/monitor.h>
#include <kernel/stdio.h>

int	monitor_init() {
	monitor_clear();
	monitor_set_color(BLACK_GREY);
	printk("kernel ver %s\n", VERSION);
	return 0;
}

