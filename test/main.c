/**
 * test/main.c - main for test
 * System sources under license MIT
 */

#include <kernel/stdio.h>
#include <drivers/monitor.h>

extern int test_memccpy();

void	klog(int status, char *msg) {
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

int	main(int ac, char **av) {
	int result;
	
	result = test_memccpy();
	klog(result, "test memccpy()");
	return 0;
}

