/**
 * Screen init
 */

#include "netero.h"
#include "drivers/screen.h"

/**
 * Note
 * just clean the screen and set a color
 * also print the version and that it
 */
int	init_screen() {
	screen_clear();
	screen_set_color(BLACK_GREY);
	printk("kernel ver ");
	printk(VERSION);
	printk("\n");
	return 0;
}

