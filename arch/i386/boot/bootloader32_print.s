[BITS 32]

;; --------------------------------------
;; bootloader32_print.s - print proc for 32 bits mode
;; System sources under license MIT
;; --------------------------------------

VIDEO_BUFFER	equ 0xb8000	;; Viedo memory buffer
WB		equ 0x0f	;; White over Black

;;	print a string
;;	esi -> buffer to write
;;	esi will be move to the end of the buffer
;;	tjhe buffer must end white 0x00
print32_str:
	pusha
	mov	edx, VIDEO_BUFFER
.loop	mov	al, [esi]
	mov	ah, WB
	cmp	al, 0x00
	je	.end
	mov	[edx], ax
	inc	esi
	add	edx, 2
	jmp	.loop
.end:	popa
	ret

