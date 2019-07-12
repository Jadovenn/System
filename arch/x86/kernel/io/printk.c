/**
 * printk.c - print formated text to the monitor
 * System sources under license MIT
 */

#include <kernel/types.h>
#include <kernel/va_arg.h>
#include <drivers/monitor.h>

void	__print_string(char *str) {
	for (char *c = str; *c; c++) {
		monitor_write(*c);
	}
}

void	__print_decimal(int number) {
	if (number == 0) {
		monitor_write('0');
	}
	else if (number < 0) { 
		monitor_write('-');
		number = -1;
	}
	else {
		int digit = number % 10;
		int next = number / 10;
		if (next != 0) {
			__print_decimal(next);
		}
		monitor_write((char)digit + 48);
	}
}

void	__print_hexadecimal(uint32_t number) {
	int counter = 0;
	char	hex_char[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
	while (number != 0 && counter < 8) {
		int temp = 0;
		temp = number % 16;
		if (temp < 10) {
			hex_char[counter] = temp + 48;
		}
		else {
			hex_char[counter] = temp + 55;
		}
		counter += 1;
		number = number / 16;
	}
	counter = 7;
	while (counter >= 0) {
		monitor_write(hex_char[counter]);
		counter -= 1;
	}
}

int	__print_formated(va_list *vl, char *format) {
	char *string;
	char character;
	int  decimal_number;
	uint32_t hexa_number;
	switch (*format) {
		case 's':
			string = va_arg(*vl, char*);
			__print_string(string);
			break;
		case 'c':
			character = va_arg(*vl, char);
			monitor_write(character);
			break;
		case 'd':
			decimal_number = va_arg(*vl, int);
			__print_decimal(decimal_number);
			break;
		case '#':
			monitor_write('0');
			monitor_write('x');
			format += 1;
			if (*format == 'x') {
				hexa_number = va_arg(*vl, uint32_t);
				__print_hexadecimal(hexa_number);
				return 1;
			}
			break;
		case 'x':
			decimal_number = va_arg(*vl, int);
			__print_hexadecimal(decimal_number);
			break;
		default:
			return 0;
	}
	return 0;
}

/**
 * @brief print formated text to the monitor
 * @details this does not use any buffer the formated
 * text given is directly print using the monitor driver
 * @param format - the formated sequence
 * @param ... - variadic argument list to print use by the format description
 */
void	printk(char *format, ...) {
	va_list	vl;
	va_start(vl, format);
	for (char *c = format; *c; c++) {
		if (*c == '%') {
			c += 1;
			if (*c != '\0') {
				c += __print_formated(&vl, c);
			}
		}
		else if (*c == '\\') {
			c += 1;
			if (*c != '\0') {
				monitor_write(*c);
			}
		}
		else {
			monitor_write(*c);
		}
	}
	va_end(vl);
}

