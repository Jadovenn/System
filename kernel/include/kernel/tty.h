/**
 * tty.h - default output
 * System sources undel license MIT
 */

#ifndef TTY_H_
# define TTY_H_

// From monitor driver

struct s_cursor_position {
	int	x;
	int	y;
};

void	monitor_clear();
void	monitor_set_color(unsigned char fg, unsigned char bg);
void	monitor_write(char c);
void	monitor_set_cursor_position(struct s_cursor_position *s_cp);
void	monitor_get_cursor_position(struct s_cursor_position *s_cp);

#endif // TTY_H_

