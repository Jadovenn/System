/**
 * Ports driver
 */

unsigned char	port_read_byte(unsigned short port) {
	unsigned char	result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void		port_write_byte(unsigned short port, unsigned char data) {
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short	port_read_word(unsigned short port) {
	unsigned short	result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void		port_write_word(unsigned short port, unsigned short data) {
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

