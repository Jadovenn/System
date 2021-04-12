/**
 * abort.c - abort the system or the current process
 * System sources under license MIT
 */

__attribute__((__noreturn__)) void abort(void)
{
	for (;;) {}
	__builtin_unreachable();
}
