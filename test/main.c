/**
 * test/main.c - main for test
 * System sources under license MIT
 */

#include <kernel/stdio.h>
#include <drivers/monitor.h>

extern int test_memccpy();
extern int test_strlen();
extern int test_heap_physical();
extern int test_heap_fragmentation();
extern int test_heap_big_alloc();

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
	(void)ac;
	(void)av;
	int result;

	result = test_heap_physical();
	klog(result, "test heap_physical()");
	result = test_heap_fragmentation();
	klog(result, "test heap_fragmentation()");
	result = test_heap_big_alloc();
	klog(result, "test heap_big_alloc()");
	result = test_memccpy();
	klog(result, "test memccpy()");
	result = test_strlen();
	klog(result, "test strlen()");
	return 0;
}

