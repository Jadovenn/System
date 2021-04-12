/**
 * hal/console.h - HAL for console
 * System sources under license MIT
 */

#ifndef CONSOLE_H
#define CONSOLE_H

struct Console_driver
{
	unsigned height;
	unsigned width;
	void (*write)(char);
};

void Hal_register_console_driver(struct Console_driver* driver);

void Hal_console_write(char c);

#endif // CONSOLE_H
