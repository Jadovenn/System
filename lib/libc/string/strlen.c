/**
 * strlen.c - compute string len
 * System sources under license MIT
 */

#include <bitset.h>
#include <stdint.h>
#include <string.h>

size_t strlen(const char* str) {
	const uint32_t* word_ptr = NULL;
	const char* byte_ptr = str;

	while ((((uint32_t)byte_ptr) % 4) != 0) {
		if (*byte_ptr == '\0') {
			return byte_ptr - str;
		}
		byte_ptr += 1;
	}
	word_ptr = (uint32_t*)byte_ptr;
	for (;;) {
		uint32_t word = *word_ptr;
		if (_has_zero(word)) {
			const char* c = (const char*)word_ptr;
			if (c[0] == 0) {
				return c - str;
			}
			if (c[1] == 0) {
				return c - str + 1;
			}
			if (c[2] == 0) {
				return c - str + 2;
			}
			if (c[3] == 0) {
				return c - str + 3;
			}
		}
		word_ptr += 1;
	}
}
