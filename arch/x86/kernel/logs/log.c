/**
 * log
 */

#include "drivers/monitor.h"

void	klog_init(int status, char *msg) {
	monitor_set_color(BLACK_GREY);
	printk("[");
	if (status == 0) {
		monitor_set_color(BLACK_GREEN_H);
		printk("pass");
	}
	else {
		monitor_set_color(BLACK_RED_H);
		printk("failed");
	}
	monitor_set_color(BLACK_GREY);
	printk("]");
	monitor_set_color(BLACK_WHITE);
	printk(" ");
	printk(msg);
	printk("\n");
	monitor_set_color(BLACK_GREY);
}

