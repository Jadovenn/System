/**
 * test/arch/i386/arch_test.c - i386 arch test
 * System sources under license MIT
 */

#include <test/test.h>

extern int test_physical_memory_manager();

void	arch_test() {
	int result = 0;
	result = test_physical_memory_manager();
	klog(result, "test physical_memory_manager()");
}

