;; bootloader read disk

;;	read from disk the number of given sector, cylinder 0
;;	and write readden data in the buffer [es:bx]
;;	bx -> sufficiant offset on the stack to write the data
;;	dh -> number of sector to read
;;	dl -> drive number (0 for floppydisk) usually set by the BIOS for you
read_disk_sector:
	pusha
	;; save input
	push	dx
	;; read
	mov	ah, 0x02	;; read interupt on 13h interuption
	mov	al, dh		;; number of sector to read
	mov	cl, 0x02	;; start at sector 2, 1 is the bootloader
	mov	ch, 0x00	;; cylinder to read
	mov	dh, 0x00	;; head number
	int	0x13	
	jc	.read_error	
	pop 	dx
	cmp	al, dh		;; check the number of sector readden
	jne	.sector_error
	popa
	ret
.read_error:
	mov	si, read_error_msg
	call 	print16_str
	jmp	.end_on_error
.sector_error:
	mov	si, sector_error_msg
	call	print16_str
.end_on_error:
	jmp	$

;; data
read_error_msg		db 'Error while reading from the disk', 13, 10, 0
sector_error_msg	db 'Incorect number of sector readden', 13, 10, 0

