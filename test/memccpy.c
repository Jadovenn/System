/**
 * test/memccpy.c - Test for mem charcter copy
 * System sources under license MIT
 */

#include <stdlib.h>
#include <string.h>
#include <kernel/stdio.h>

int	test_memccpy() {
	char	source[] = "This is a test.";
	char	target[15];
	
	target[14] = '\0';
	char *result = memccpy(target, source, 'a', 12);
	if (result == NULL)
		return EXIT_FAILURE;
	result = memccpy(target, source, 'x', 12);
	if (result != NULL)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

