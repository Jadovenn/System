/**
 * monitor.h - monitor driver API
 * System sources under license MIT
 */

#ifndef MONITOR_H_
# define MONITOR_H_

//#define VIDEO_MEMORY_BUFFER_PTR		((char*)0xb8000)
#define VIDEO_MEMORY_BUFFER_PTR		((char*)0xC03FF000)

#define VGA_CURSOR_POSITION_HIGHT	14
#define VGA_CURSOR_POSITION_LOW		15

enum vga_color {
	VGA_COLOR_BLACK = 0x00,
	VGA_COLOR_BLUE = 0x01,
	VGA_COLOR_GREEN = 0x02,
	VGA_COLOR_CYAN = 0x03,
	VGA_COLOR_RED = 0x04,
	VGA_COLOR_MAGENTA = 0x05,
	VGA_COLOR_BROWN = 0x06,
	VGA_COLOR_LIGHT_GREY = 0x07,
	VGA_COLOR_DARK_GREY = 0x08,
	VGA_COLOR_LIGHT_BLUE = 0x09,
	VGA_COLOR_LIGHT_GREEN = 0x0a,
	VGA_COLOR_LIGHT_CYAN = 0x0b,
	VGA_COLOR_LIGHT_RED = 0x0c,
	VGA_COLOR_LIGHT_MAGENTA = 0x0d,
	VGA_COLOR_LIGHT_BROWN = 0x0e,
	VGA_COLOR_WHITE = 0x0f,
};

static inline char vga_entry_color(unsigned char fg, unsigned char bg) {
	return fg | bg << 4;
}

/**
enum s_screen_color {
	BLACK_BLUE_STRONG = 0x01,
	BLACK_GREEN_STRONG = 0x02,
	BLACK_BLUE_SKY = 0x03,
	BLACK_RED = 0x04,
	BLACK_PURPLE = 0x05,
	BLACK_ORANGE = 0x06,
	BLACK_GREY = 0x07,
	BLACK_GREY_STRONG = 0x08,
	BLACK_GREEN_H = 0x0a,
	BLACK_BLUE_H = 0x0b,
	BLACK_RED_H = 0x0c,
	BLACK_PURPLE_H = 0x0d,
	BLACK_YELLOW = 0x0e,
	BLACK_WHITE = 0x0f,
};
*/

#endif // MONITOR_H_

