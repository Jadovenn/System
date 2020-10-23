/**
 * tty.h - default output
 * System sources undel license MIT
 */

#ifndef TTY_H_
#define TTY_H_

// From monitor driver

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

struct s_cursor_position {
	int x;
	int y;
};

void monitor_clear();
void monitor_set_color(unsigned char fg, unsigned char bg);
void monitor_write(char c);
void monitor_set_cursor_position(struct s_cursor_position* s_cp);
void monitor_get_cursor_position(struct s_cursor_position* s_cp);

#endif // TTY_H_
