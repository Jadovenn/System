/**
 * link.ld
 * linker script make sure every things is where
 * it is supposed to be.
 * it define code, data, bss and end pointer needed by the kernel
 */

ENTRY(start)
SECTIONS
{

    .text 0x1000 :
    {
        code = .; _code = .; __code = .;
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        data = .; _data = .; __data = .;
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss :
    {
        bss = .; _bss = .; __bss = .;
        *(.bss)
        . = ALIGN(4096);
    }

    end = .; _end = .; __end = .;
}

