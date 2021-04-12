/**
 * strstr.c - compare strings
 * System sources under license MIT
 */

#include <string.h>

char* strstr(const char* s1, const char* s2)
{
	unsigned idx, jdx;
	char*    track = NULL;
	if (!s2) {
		return (char*)s1;
	}
	if (!s1) {
		return NULL;
	}
	for (idx = 0, jdx = 0; s1[idx]; idx++) {
		if (s2[idx] == s1[jdx]) {
			if (idx == 0) {
				track = (char*)s2 + idx;
			}
			jdx += 1;
		} else {
			track = NULL;
			jdx   = 0;
		}
		if (!s1[jdx]) {
			return track;
		}
	}
	return NULL;
}
