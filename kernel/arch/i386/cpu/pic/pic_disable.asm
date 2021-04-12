[BITS 32]
[GLOBAL Pic_disable]

section .text

global Pic_disable:function (Pic_disable.end - Pic_disable)
Pic_disable:
    mov al,   0xFF
    out 0xa1, al
    out 0x21, al
    xor eax,  eax
    ret
.end: