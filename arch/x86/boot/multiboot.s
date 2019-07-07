[BITS 32]
;; ------------------------------------
;; multiboot.s - multiboot kernel entry point
;; System sources under license MIT
;; ------------------------------------

;; Constant
MBALIGN  equ	1 << 0
MEMINFO	 equ	1 << 1
FLAGS	 equ	MBALIGN | MEMINFO
MAGIC	 equ	0x1badb002
CHECKSUM equ	-(MAGIC + FLAGS)

;; Multiboot header
section	.multiboot
align 4
	dd	MAGIC
	dd	FLAGS
	dd	CHECKSUM

;; Stack 16Kib
section .bss
align	16
stack_bottom:
resb	16384
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
	mov	esp, stack_top
	extern	kernel_main_multiboot
	call	kernel_main_multiboot
	cli
.hang:	hlt
	jmp .hang
.end:

