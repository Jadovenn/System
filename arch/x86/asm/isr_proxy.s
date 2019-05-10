[BITS 32]
[EXTERN isr_handler]
[global isr_proxy]

;; isr proxy, save processor context, set up kernel mods segment, call hight
;; level kernel fault handler. Finally restores stack frame

isr_proxy:
	pusha	;;save edi, esi, ebp, esp, ebx, edx, ecx, eax
	mov	ax, ds
	push	eax

	mov	ax, 0x10 ;; kernel data segment
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	call	isr_handler

	pop eax
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	popa
	add	esp, 8
	sti
	iret

