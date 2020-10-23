/**
 * monitor_init.c - clear monitor and print kernel version
 * System sources under license MIT
 */

#include <kernel/stdio.h>
#include <kernel/tty.h>

#include <system.h>

int monitor_driver_init() {
	monitor_clear();
	monitor_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	printk("kernel ver %s\n", VERSION);
	return 0;
}
