[BITS 16]

;; --------------------------------------
;; bootloader_32bits_switch.s - proc to set cpu in 32 bits mode
;; System sources under license MIT
;; --------------------------------------

switch_to_protected_mode:
	cli	;; disable interuption
	lgdt	[gdt_descriptor]
	mov	eax, cr0
	or	eax, 0x1
	mov	cr0, eax	;;enable 32 bit
	jmp	CODE_SEGMENT:init_protected_mode

[BITS 32]
init_protected_mode:
	;; set data segment
	mov	ax, DATA_SEGMENT
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	;; create stack at the begining of the free data
	mov	ebp, 0x90000
	mov	esp, ebp
	call	_start_protected_mode

