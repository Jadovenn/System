/**
 * monitor_init.c - clear monitor and print kernel version
 * System sources under license MIT
 */

#include <hal/console.h>
#include <kernel/stdio.h>

#include "monitor.h"

static struct Console_driver Monitor_driver;

int monitor_driver_init()
{
	monitor_clear();
	monitor_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	Monitor_driver.height = 80;
	Monitor_driver.width  = 25;
	Monitor_driver.write  = &monitor_write;
	Hal_register_console_driver(&Monitor_driver);
	return 0;
}
