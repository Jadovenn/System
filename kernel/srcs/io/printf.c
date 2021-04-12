/**
 * printk.c - print formated text to the monitor
 * System sources under license MIT
 */

#include <stdarg.h>
#include <stdint.h>

#include <hal/console.h>

// TODO: This implementation could be improved in many way.
// first, some argument does not match what they should really do (l actually
// match lu) second, __print_format should be improve to accept easly more than
// one arguments third, print hexa could be better, others too

void _print_string(char* str)
{
	for (char* c = str; *c; c++) {
		Hal_console_write(*c);
	}
}

void _print_decimal(int number)
{
	if (number == 0) {
		Hal_console_write('0');
	} else if (number < 0) {
		Hal_console_write('-');
		number = -1;
	} else {
		int digit = number % 10;
		int next  = number / 10;
		if (next != 0) {
			_print_decimal(next);
		}
		Hal_console_write((char)digit + 48);
	}
}

void _print_unsigned_decimal(unsigned number)
{
	if (number == 0) {
		Hal_console_write('0');
	} else {
		unsigned digit = number % 10;
		unsigned next  = number / 10;
		if (next != 0) {
			_print_decimal(next);
		}
		Hal_console_write((char)digit + 48);
	}
}

void _print_long_decimal(int64_t number)
{
	if (number == 0) {
		Hal_console_write('0');
	} else {
		uint64_t digit = number % 10;
		uint64_t next  = number / 10;
		if (next != 0) {
			_print_long_decimal(next);
		}
		Hal_console_write((char)digit + 48);
	}
}

void _print_hexadecimal(uint32_t number)
{
	int  counter     = 0;
	char hex_char[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
	while (number != 0 && counter < 8) {
		int temp = 0;
		temp     = number % 16;
		if (temp < 10) {
			hex_char[counter] = temp + 48;
		} else {
			hex_char[counter] = temp + 55;
		}
		counter += 1;
		number = number / 16;
	}
	counter = 7;
	while (counter >= 0) {
		Hal_console_write(hex_char[counter]);
		counter -= 1;
	}
}

int _print_formated(va_list* vl, const char* format)
{
	char*    string;
	char     character;
	int      decimal_number;
	unsigned unsigned_number;
	uint64_t long_number;
	uint32_t hexa_number;
	switch (*format) {
	case 's':
		string = va_arg(*vl, char*);
		_print_string(string);
		break;
	case 'c':
		character = va_arg(*vl, char);
		Hal_console_write(character);
		break;
	case 'd':
		decimal_number = va_arg(*vl, int);
		_print_decimal(decimal_number);
		break;
	case 'u':
		unsigned_number = va_arg(*vl, unsigned);
		_print_unsigned_decimal(unsigned_number);
		break;
	case 'l':
		long_number = va_arg(*vl, uint64_t);
		_print_long_decimal(long_number);
		break;
	case '#':
		Hal_console_write('0');
		Hal_console_write('x');
		format += 1;
		if (*format == 'x') {
			hexa_number = va_arg(*vl, uint32_t);
			_print_hexadecimal(hexa_number);
			return 1;
		}
		break;
	case 'x':
		decimal_number = va_arg(*vl, int);
		_print_hexadecimal(decimal_number);
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
void printf(char const* format, ...)
{
	va_list vl;
	va_start(vl, format);
	for (const char* c = format; *c; c++) {
		if (*c == '%') {
			c += 1;
			if (*c != '\0') {
				c += _print_formated(&vl, c);
			}
		} else if (*c == '\\') {
			c += 1;
			if (*c != '\0') {
				Hal_console_write(*c);
			}
		} else {
			Hal_console_write(*c);
		}
	}
	va_end(vl);
}
