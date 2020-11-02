/**
 * link.ld - linker srcipt, define sctions and txt start addr
 * System sources under license MIT
 */

ENTRY(_start)
SECTIONS {

	G_Kernel_space = 0xC0100000;

	. = 0xC0100000;

	G_Start_kernel = .;

   	.text ALIGN (4K) : AT(ADDR(.text) - 0xC0000000) {
		*(.multiboot)
		G_Start_code = .;
		*(.text)
		G_End_code = .;
    }

	G_Start_rodata = .;

	.rodata ALIGN (4K) : AT(ADDR(.rodata) - 0xC0000000) {
		*(.rodata)
	}

	G_End_rodata = .;
	G_Start_data = .;

	.data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}

	G_End_data = .;
	G_Start_bss = .;

	.bss ALIGN (4K) : AT(ADDR(.bss) - 0xC0000000) {
		*(COMMON)
		*(.bss)
	}

	G_End_bss = .;
	G_End_kernel = .;

}
