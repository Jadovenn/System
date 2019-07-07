[BITS 32]
[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]
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
mboot:
	dd	MAGIC
	dd	FLAGS
	dd	CHECKSUM
	dd	mboot
	dd	code
	dd	bss
	dd	end
	dd	_start

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
	push	ebx
	call	kernel_main_multiboot
	cli
.hang:	hlt
	jmp .hang
.end:

