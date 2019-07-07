/**
 * link.ld - linker srcipt, define sctions and txt start addr
 * System sources under license MIT
 */

ENTRY(_start)
SECTIONS {
	. = 1M;
	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
	}

    	.text  BLOCK(4K) : ALIGN(4K) {
		code = .;
		*(.text)
    	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}
 
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}
 
	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		bss = .;
		*(.bss)
	}
	end = .; 
}

