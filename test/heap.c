/**
 * test/heap.c - test heap
 * System sources under license MIT
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <kernel/heap.h>

int	test_heap_physical() {
	uint32_t *data_1 = NULL;
	uint32_t *data_2 = NULL;
	uint32_t physical_addr_1;
	uint32_t physical_addr_2;

	// compute physcial address from virtual
	data_1 = kmalloc_physical(sizeof(uint32_t), &physical_addr_1);
	if (!data_1) {
		return EXIT_FAILURE;
	}
	if (((uint32_t)data_1) - 0xC0000000 != physical_addr_1) {
		kfree(data_1);
		return EXIT_FAILURE;
	}
	kfree(data_1);
	data_1 = kmalloc_physical(sizeof(uint32_t) * 100, &physical_addr_1);
	if (!data_1) {
		return EXIT_FAILURE;
	}
	data_2 = kmalloc_physical(sizeof(uint32_t) * 3, &physical_addr_2);
	if (!data_2) {
		kfree(data_1);
		return EXIT_FAILURE;
	}
	if (((uint32_t)data_1) - 0xC0000000 != physical_addr_1) {
		kfree(data_1);
		kfree(data_2);
		return EXIT_FAILURE;
	}
	if (((uint32_t)data_2) - 0xC0000000 != physical_addr_2) {
		kfree(data_1);
		kfree(data_2);
		return EXIT_FAILURE;
	}
	kfree(data_1);
	kfree(data_2);
	return EXIT_SUCCESS;
}

int	test_heap_fragmentation() {
	uint32_t	*data_1 = NULL;
	uint32_t	*data_2 = NULL;
	uint32_t	*data_3 = NULL;

	// Make Fragmentation work
	data_1 = kmalloc(sizeof(uint32_t));
	if (!data_1) {
		return EXIT_FAILURE;
	}
	data_2 = kmalloc(sizeof(uint32_t));
	if (!data_2) {
		kfree(data_1);
		return EXIT_FAILURE;
	}
	data_3 = kmalloc(sizeof(uint32_t));
	if (!data_3) {
		kfree(data_1);
		kfree(data_2);
		return EXIT_FAILURE;
	}
	*data_1 = 42;
	kfree(data_2);
	kfree(data_1);
	kfree(data_3);
	data_1 = kmalloc(sizeof(uint32_t));
	if (!data_1) {
		return EXIT_FAILURE;
	}
	if (*data_1 != 42) {
		kfree(data_1);
		return EXIT_FAILURE;
	}
	kfree(data_1);
	return EXIT_SUCCESS;
}

int	test_heap_big_alloc() {
	int result = EXIT_SUCCESS;
	int block_size = sizeof(uint32_t);
	int block_total_nb = 8192 / block_size;
	int block_nb = 0;
	uint32_t *ptr = NULL;
	uint32_t *it = NULL;

	// Make paging system work
	it = kmalloc(block_size);
	if (!it) {
		return EXIT_FAILURE;
	}
	ptr = it;
	block_nb += 1;
	for (; block_nb < block_total_nb; block_nb += 1) {
		uint32_t *block = kmalloc(block_size);
		if (!it) {
			result = EXIT_FAILURE;
			break;
		}
		*it = (uint32_t)block;
		it = block;
	}
	while (ptr != it) {
		uint32_t *next = (uint32_t*)*ptr;
		kfree(ptr);
		ptr = next;
	}
	kfree(ptr);
	return result;
}

