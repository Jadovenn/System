/**
 * log
 */

#include <drivers/monitor.h>
#include <kernel/stdio.h>

void	klog_init(int status, char *msg) {
	monitor_set_color(BLACK_GREY);
	monitor_write('[');
	if (status == 0) {
		monitor_set_color(BLACK_GREEN_H);
		printk("pass");
	}
	else {
		monitor_set_color(BLACK_RED_H);
		printk("failed");
	}
	monitor_set_color(BLACK_GREY);
	monitor_write(']');
	monitor_set_color(BLACK_WHITE);
	monitor_write(' ');
	printk(msg);
	monitor_write('\n');
	monitor_set_color(BLACK_GREY);
}

