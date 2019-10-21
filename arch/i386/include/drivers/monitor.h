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

struct s_cursor_position {
	int	x;
	int	y;
};

void	monitor_write(char c);
void	monitor_clear();
void	monitor_set_color(char color);
void	monitor_set_cursor_position(struct s_cursor_position *s_cp);
void	monitor_get_cursor_position(struct s_cursor_position *s_cp);

#endif // MONITOR_H_
