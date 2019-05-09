/**
 * VGA - screen driver
 */

#include "kernel/ports.h"
#include "kernel/io.h"
#include "drivers/monitor.h"
#include "string.h"

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
	offset = offset << 1;
	return offset;
}

static void	set_cursor_offset(int offset) {
	offset = offset >> 1;
	port_write_byte(VGA_SCREEN_CTRL, VGA_CURSOR_POSITION_HIGHT);
	port_write_byte(VGA_DATA_REG, (unsigned char)(offset >> 8));
	port_write_byte(VGA_SCREEN_CTRL, VGA_CURSOR_POSITION_LOW);
	port_write_byte(VGA_DATA_REG, (unsigned char)(offset & 0xff));
	offset = get_cursor_offset();
}

static void	scroll_down() {
	int	dest_offset = (COLONE_MAX * ROW_MAX - COLONE_MAX) << 1;
	for (unsigned count = 0; count < dest_offset; count += COLONE_MAX << 1) {
		memcpy(VIDEO_MEMORY_BUFFER_PTR + count,
				VIDEO_MEMORY_BUFFER_PTR + count + (COLONE_MAX << 1),
				COLONE_MAX << 1);
	}
	int offset = dest_offset;
	dest_offset = (COLONE_MAX * ROW_MAX) << 1;
	while (offset < dest_offset) {
		VIDEO_MEMORY_BUFFER_PTR[offset] = 0x00;
		VIDEO_MEMORY_BUFFER_PTR[offset + 1] = 0x0f;
		offset += 2;
	}
} // O(n) = COLONE_MAX * ROW_MAX

void	monitor_set_cursor_position(struct s_cursor_position *s_cp) {
	int 	offset = get_offset_from_chartesian(s_cp->x, s_cp->y);
	set_cursor_offset(offset);
}

void	printk_at(char c, int x, int y) {
	struct s_cursor_position	cp;
	cp.x = x;
	cp.y = y;
	monitor_set_cursor_position(&cp);
	printk_char(c);
}

/**
 * Note
 * Manage overflow and scrolling here
 */
void	printk_char(char c) {
	int offset = get_cursor_offset();
	if ((offset >> 1) % COLONE_MAX == COLONE_MAX - 1) { // Special Case: border of the screen
		if ((offset >> 1) / COLONE_MAX == 24) { // Special Case: bottom of the screen
			VIDEO_MEMORY_BUFFER_PTR[offset] = '\\';
			VIDEO_MEMORY_BUFFER_PTR[offset + 1] = cursor_color;
			scroll_down();
			offset = (COLONE_MAX * ROW_MAX - COLONE_MAX) * 2;
			set_cursor_offset(offset);
			printk_char(c);
		}
		else { // Special Case: border of the screen, perform carriage return
			VIDEO_MEMORY_BUFFER_PTR[offset] = '\\';
			VIDEO_MEMORY_BUFFER_PTR[offset + 1] = cursor_color;
			set_cursor_offset(offset + 2);
			printk_char(c);
		}
	}
	else if (c == '\n') { // Special Case: carriage return
		if ((offset >> 1) / COLONE_MAX == 24) { // Special Case: bottom of the screen
			scroll_down();
			offset = offset - (offset % COLONE_MAX);
			set_cursor_offset(offset);
		}
		else { // Special Case: carriage return
			offset = offset - (offset % COLONE_MAX);
			set_cursor_offset(offset + (2 * COLONE_MAX));
		}
	}
	else { // Regular Case
		VIDEO_MEMORY_BUFFER_PTR[offset] = c;
		VIDEO_MEMORY_BUFFER_PTR[offset + 1] = cursor_color;
		set_cursor_offset(offset + 2);
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

void	monitor_set_color(char color) {
	cursor_color = (char)color;
}

void	monitor_clear() {
	for (int counter = 0; counter < COLONE_MAX * ROW_MAX; counter++) {
		VIDEO_MEMORY_BUFFER_PTR[counter << 1] = 0x00;
		VIDEO_MEMORY_BUFFER_PTR[(counter << 1) + 1] = 0x0f;
	}
	set_cursor_offset(0);
} // O(n) = COLONE_MAX * ROW_MAX

