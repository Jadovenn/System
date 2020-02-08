/**
 * link.ld - linker srcipt, define sctions and txt start addr
 * System sources under license MIT
 */

ENTRY(_start)
SECTIONS {

	_kernel_space = 0xC0100000;

	. = 0xC0100000;

	_kernel_start = .;
    	.text ALIGN (4K) : AT(ADDR(.text) - 0xC0000000) {
		*(.multiboot)
		_code = .;
		*(.text)
    	}
	_end_code = .;
	_rodata = .;
	.rodata ALIGN (4K) : AT(ADDR(.rodata) - 0xC0000000) {
		*(.rodata)
	}
	_end_rodata = .;
	.data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}
	_end_data = .;
	.bss ALIGN (4K) : AT(ADDR(.bss) - 0xC0000000) {
		*(COMMON)
		_bss = .;
		*(.bss)
	}
	_end = .; 
}

