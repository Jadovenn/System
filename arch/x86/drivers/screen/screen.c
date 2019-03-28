/**
 * VGA - screen driver
 */

#include "kernel/ports.h"
#include "kernel/io.h"
#include "drivers/screen.h"

#define	COLONE_MAX	80
#define ROW_MAX		25

static char	cursor_color = 0x0f;

/**
 * Note
 * This will need to be improve while files would be ready, so 
 * we will implement this function as standard library function using
 * a kwrite function and connect this driver to a file and print the content comming
 * fromt the file. this file would be the standard output of our kernel, maybe tty later?
 */

static int	get_offset_from_chartesian(int x, int y) {
	return (2 * (y * COLONE_MAX + x));
}

static int	get_cursor_offset() {
	int	offset = 0;
	port_write_byte(VGA_SCREEN_CTRL, VGA_CURSOR_POSITION_HIGHT);
	offset = port_read_byte(VGA_DATA_REG);
	offset = offset << 8;
	port_write_byte(VGA_SCREEN_CTRL, VGA_CURSOR_POSITION_LOW);
	offset += port_read_byte(VGA_DATA_REG);
	offset *= 2;
	return offset;
}

static void	set_cursor_offset(int offset) {
	offset = offset / 2;
	port_write_byte(VGA_SCREEN_CTRL, VGA_CURSOR_POSITION_HIGHT);
	port_write_byte(VGA_DATA_REG, (unsigned char)(offset >> 8));
	port_write_byte(VGA_SCREEN_CTRL, VGA_CURSOR_POSITION_LOW);
	port_write_byte(VGA_DATA_REG, (unsigned char)(offset & 0xff));
	offset = get_cursor_offset();
}

void	screen_set_cursor_position(struct s_cursor_position *s_cp) {
	int 	offset = get_offset_from_chartesian(s_cp->x, s_cp->y);
	set_cursor_offset(offset);
}

void	printk_at(char c, int x, int y) {
	struct s_cursor_position	cp;
	cp.x = x;
	cp.y = y;
	screen_set_cursor_position(&cp);
	printk_char(c);
}

/**
 * Note
 * Manage overflow and scrolling here
 */
void	printk_char(char c) {
	int offset = get_cursor_offset();
	if (c == '\n') {
		offset = offset - (offset % COLONE_MAX);
		set_cursor_offset(offset + (2 * COLONE_MAX));
	}
	else {
		VIDEO_MEMORY_BUFFER_PTR[offset] = c;
		VIDEO_MEMORY_BUFFER_PTR[offset + 1] = cursor_color;
		set_cursor_offset(offset+2);
	}
}

/**
 * Note
 * This will manage only string until, a better version is written.
 * If you want to do it make it looks like printf
 */
void	printk(char *s) {
	for (register unsigned i = 0; s[i]; i++)
		printk_char(s[i]);
}

void	screen_set_color(char color) {
	cursor_color = (char)color;
}

void	screen_clear() {
	for (int counter = 0; counter < COLONE_MAX * ROW_MAX; counter++) {
		VIDEO_MEMORY_BUFFER_PTR[counter * 2] = 0x00;
		VIDEO_MEMORY_BUFFER_PTR[counter * 2 + 1] = 0x0f;
	}
	set_cursor_offset(0);
}

