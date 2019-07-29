/**
 * strlen.c - compute string len
 * System sources under license MIT
 */

#include <string.h>

size_t	strlen(const char *s) {
	size_t count = 0;
	while (s[count] != '\0')
		count += 1;
	return count;
}

