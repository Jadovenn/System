[BITS 32]
[GLOBAL _init]

;; -----------------------------------------------------
;; init.s - kernel early control register initialization
;; System sources under license MIT
;; -----------------------------------------------------

%define	CR4_PSE	0x00000010

section .text
_init:
	push	eax
	;; set page size up to 4Mib
	mov	eax, cr4
	or	eax, CR4_PSE
	mov	cr4, eax
	pop	eax
	ret

