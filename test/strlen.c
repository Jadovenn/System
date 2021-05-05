/**
 * test/strlen.c - test strlen
 * System sources under license MIT
 */

#include <stdlib.h>
#include <string.h>
#include <kernel/heap.h>
#include <kernel/stdio.h>

int	test_strlen() {
	char	str_1[] = "hello world";
	char	*str_2;
	size_t	len;


	len = strlen(str_1);
	if (len != 11) {
		return EXIT_FAILURE;
	}

	len = strlen(str_1 + 1);
	if (len != 10) {
		return EXIT_FAILURE;
	}

	str_2 = kmalloc(2048 * sizeof(char));
	if (!str_2) {
		printk("Error kmalloc\n");
		return EXIT_FAILURE;
	}
	for (unsigned idx = 0; idx < 2048; idx++)
		str_2[idx] = 'a';

	str_2[2047] = 0;
	len = strlen(str_2);
	if (len != 2047) {
		kfree(str_2);
		return EXIT_FAILURE;
	}

	str_2[2000] = 0;
	len = strlen(str_2);
	if (len != 2000) {
		kfree(str_2);
		return EXIT_FAILURE;
	}

	str_2[1533] = 0;
	len = strlen(str_2);
	if (len != 1533) {
		kfree(str_2);
		return EXIT_FAILURE;
	}

	str_2[3] = 0;
	len = strlen(str_2);
	if (len != 3) {
		kfree(str_2);
		return EXIT_FAILURE;
	}
	kfree(str_2);
	return EXIT_SUCCESS;
}

