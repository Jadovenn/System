/**
 * div.c - Compute the quotien and reminder of a division
 * System under license MIT
 */

#include <stdlib.h>

struct div_t div(int number, int denom) {
	struct div_t result;
	result.quot = number / denom;
	result.rem = number % denom;
	return result;
}

struct ldiv_t ldiv(long number, long denom) {
	struct ldiv_t result;
	result.quot = number / denom;
	result.rem = number % denom;
	return result;
}

struct lldiv_t lldiv(long long number, long long denom) {
	struct lldiv_t result;
	result.quot = number / denom;
	result.rem = number % denom;
	return result;
}
