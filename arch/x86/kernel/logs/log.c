/**
 * log
 */

#include "drivers/screen.h"

void	klog_init(int status, char *msg) {
	screen_set_color(BLACK_GREY);
	printk("[");
	if (status == 0) {
		screen_set_color(BLACK_GREEN_H);
		printk("pass");
	}
	else {
		screen_set_color(BLACK_RED_H);
		printk("failed");
	}
	screen_set_color(BLACK_GREY);
	printk("]");
	screen_set_color(BLACK_WHITE);
	printk(" ");
	printk(msg);
	printk("\n");
	screen_set_color(BLACK_GREY);
}

