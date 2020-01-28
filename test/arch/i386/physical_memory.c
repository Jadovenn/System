/**
 * test/arch/i386/physical_memory.c - Physical Memory Manager
 * System sources under license MIT
 */

#include <arch/physical_memory.h>

int	test_pyshical_memory_manager() {
	void *page = pmm_alloc();
	unsigned idx = 0;
	while (pmm_alloc()) {
		idx += 1;
	}
	printk("allocated page: %#x\n", (uint32_t)page + idx * 0x1000);
	while (idx) {
		pmm_free((void*)((uint32_t)page + idx * 0x1000));
		idx -= 1;
	}
	pmm_free(page);
}

