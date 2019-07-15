/**
 * link.ld - linker srcipt, define sctions and txt start addr
 * System sources under license MIT
 */

ENTRY(_start)
SECTIONS {

	. = 0xC0100000;

	_kernel_start = .;
    	.text ALIGN (4K) : AT(ADDR(.text) - 0xC0000000) {
		*(.multiboot)
		code = .;
		*(.text)
    	}
	ecode = .;
	.rodata ALIGN (4K) : AT(ADDR(.rodata) - 0xC0000000) {
		*(.rodata)
	}
	.data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}
	edata = .;
	.bss ALIGN (4K) : AT(ADDR(.bss) - 0xC0000000) {
		*(COMMON)
		bss = .;
		*(.bss)
	}
	end = .; 
}

