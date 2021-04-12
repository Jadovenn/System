/**
 * stdlib.h - Standard library definition
 * System sources under license MIT
 */

#ifndef STDLIB_H_
#define STDLIB_H_

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

struct div_t
{
	int quot;
	int rem;
};

struct ldiv_t
{
	long quot;
	long rem;
};

struct lldiv_t
{
	long long quot;
	long long rem;
};

struct div_t   div(int number, int denom);
struct ldiv_t  ldiv(long number, long denom);
struct lldiv_t lldiv(long long number, long long denom);

__attribute__((__noreturn__)) void abort(void);

#endif // STDLIB_H_
