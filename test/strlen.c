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
	char	str_2[] = "lkk_qwje_oqwe=-1kjbakl*kwjehlq;w4elqk kjk1kjk5kkjfe0j4bktbkew0fiwjblj34035ikb";
	size_t	len;


	len = strlen(str_1);
	if (len != 11) {
		return EXIT_FAILURE;
	}

	len = strlen(str_1 + 1);
	if (len != 10) {
		return EXIT_FAILURE;
	}

	len = strlen(str_2);
	if (len != 77) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

