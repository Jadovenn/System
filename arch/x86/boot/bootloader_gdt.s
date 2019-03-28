[BITS 32]

;; --------------------------------------
;; boatloader_gdt.s - set a tmp gdt to be able to jmp to c compiled code
;;		      the gdt is reset by the kernel later
;; System sources under license MIT
;; --------------------------------------

gdt_start:	;; A null gdt_segment as first
	dd	0x0 ;; 4 bytes null
	dd	0x0 ;; 4 bytes null
gdt_code_segment: ;; code segment descriptor
	dw	0xffff		;; segment length 0-15
	dw	0x0		;; segment base   0-15
	db	0x0		;; segment base   16-23
	db	10011010b	;; flags (8-bits)
	db	11001111b	;; flags (4-bits) + segment length, bits 16-19
	db	0x0		;; segment base	  24-31
gdt_data_segment:
	dw	0xffff
	dw	0x0
	db	0x0
	db	10010010b
	db	11001111b
	db	0x0
gdt_end:

gdt_descriptor:
	dw	gdt_end - gdt_start - 1 ;; (16 bits)
	dd	gdt_start		;; (32 bits)

CODE_SEGMENT	equ	gdt_code_segment - gdt_start
DATA_SEGMENT	equ	gdt_data_segment - gdt_start

