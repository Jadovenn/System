/**
 * test/memccpy.c - Test for mem charcter copy
 * System sources under license MIT
 */

#include <stdlib.h>
#include <string.h>

int	test_memccpy() {
	char	source[] = "This is a test.";
	char	target[10];
	
	target[9] = '\0';
	char *result = memccpy(target, source, 'a', 12);
	if (result == NULL)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

