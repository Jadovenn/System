/**
 * hal/console.c - Console hardware abstract layer
 * System sources under license MIT
 */

#include <unistd.h>

#include <hal/console.h>
#include <kernel/panic.h>

static struct Console_driver* G_Console_driver = NULL;

void Hal_register_console_driver(struct Console_driver* driver) { G_Console_driver = driver; }

void Hal_console_write(char c)
{
	if (G_Console_driver == NULL) {
		abort();
	}
	G_Console_driver->write(c);
}
