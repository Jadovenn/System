;; bootloader.s
[BITS 16]
[ORG 0x7c00]

KERNEL_OFFSET	equ	0x1000

	jmp	_start
	%include "./boot/bootloader_print.s"
	%include "./boot/bootloader_read_disk.s"

;;	Bootloader start here
_start:
	mov	[boot_drv], dl	;; save the boot disk - set by the BIOS
	;; Setup the stack
	mov	bp, 0x9000
	mov	sp, bp

	;; print lauching message
	mov	si, start_msg
	call	print16_str
	;; load the kernel
	call	load_kernel
	;; move to protected mode
	call	switch_to_protected_mode
	jmp	$ ;; never supposed to be executed or somthing went really wrong

load_kernel:
	mov	si, load_msg
	call	print16_str

	mov	bx, KERNEL_OFFSET	;; write the data at 0x1000
	mov	dh, 0x02		;; load 2 sectors	
	mov	dl, [boot_drv]		;; from the given boot_drive
	call	read_disk_sector
	ret

print16_debug:
	pusha
	mov	si, debug
	call	print16_str
	popa
	ret

carriage16_return:
	pusha
	mov	si, cr_char
	call	print16_str
	popa
	ret

	;; Begin of the 32bits region
	%include "./boot/bootloader_32bits_switch.s"
	%include "./boot/bootloader_gdt.s"
	%include "./boot/bootloader32_print.s"

[BITS 32]
_start_protected_mode:
	;; execute the kernel
	call	KERNEL_OFFSET
	jmp	$ ;; in case the kernel return, fall in an infinit loop

;; Data - 16 bits
boot_drv	db 0
start_msg	db 'Booting Netero...', 13, 10, 0
load_msg	db 'Loading Netero kernel...', 13, 10, 0
debug		db 'debug', 13, 10, 0
cr_char		db 13, 10, 0

; Fill the rest of the boot loader with 0
times 510-($-$$) db 0
;; Magic number for the BIOS
dw 0xaa55

