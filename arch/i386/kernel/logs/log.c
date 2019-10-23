/**
 * log.c - init log
 * System sources under license MIT
 */

#include <kernel/tty.h>
#include <drivers/monitor.h>
#include <kernel/stdio.h>

void	klog_init(int status, char *msg) {
	monitor_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	monitor_write('[');
	if (status == 0) {
		monitor_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
		printk("pass");
	}
	else {
		monitor_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
		printk("failed");
	}
	monitor_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	monitor_write(']');
	monitor_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	monitor_write(' ');
	printk(msg);
	monitor_write('\n');
	monitor_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

