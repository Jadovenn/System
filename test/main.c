/**
 * test/main.c - main for test
 * System sources under license MIT
 */

#include <test/test.h>
#include <kernel/stdio.h>
#include <kernel/tty.h>

extern int test_memccpy();
extern int test_strlen();
extern int test_heap_physical();
extern int test_heap_fragmentation();
extern int test_heap_big_alloc();

void	klog(int status, char *msg) {
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

int	main(int ac, char **av) {
	(void)ac;
	(void)av;
	int result;

	arch_test();
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

