/**
 * printk, print formated text in kernel space
 * it is not perfec but this would be good 
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

void	__print_formated(va_list *vl, char *format) {
	char *string;
	char character;
	int  decimal_number;
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
		default:
			return;
	}
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
				__print_formated(&vl, c);
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

