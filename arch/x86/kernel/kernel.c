/**
 *	Kernel entry point
 */

#include "kernel/ports.h"

void	kmain() {
	int	position;
	port_write_byte(0x3d4, 14);
	position = port_read_byte(0x3d5);
	position = position << 8;
	port_write_byte(0x3d4, 15);
	position += port_read_byte(0x3d5);
	int offset_from_vga = position * 2;

	char *vga = (char*)0xb8000;
	vga[offset_from_vga] = 'J';
	vga[offset_from_vga+1] = 0x0f; /* White text on black background */
}

