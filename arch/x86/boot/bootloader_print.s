[BITS 16]
;; --------------------------------------
;; bootloader_print.s - print proc for 16 bit mode
;; System sources under license MIT
;; --------------------------------------

;;	print a string
;;	put the first byte in si
;;	the string must end by 0x00
;;	si will be move to the end of the string
print16_str:
	pusha
	mov	ah, 0x0e
.loop:	mov	al, [si]
	cmp	al, 0x00
	je	.end
	int	0x10
	inc	si
	jmp	.loop
.end:	popa
	ret

;;	print an hexadecimal number
;;	put the number in dx
print16_hex:
	pusha
	xor	cx, cx
.loop:	cmp	cx, 0x04
	je	.end
	mov	ax, dx
	and	ax, 0x000f
	add	al, 0x30
	cmp	al, 0x39
	jle	.s2
	add	al, 0x07
.s2:	mov	bx, hex_out + 0x05
	sub	bx, cx
	mov	[bx], al
	ror	dx, 4
	inc	cx
	jmp	.loop
.end:	mov	si, hex_out
	call	print16_str
	popa
	ret
;;	data for print_hex
hex_out	db '0x0000', 0

