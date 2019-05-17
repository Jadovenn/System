[BITS 32]
[EXTERN isr_stub]
[EXTERN irq_stub]

%macro ISR_NO_ERROR_CODE 1
[global isr%1]
isr%1:
	cli
	push byte 0 ;; push dummy error code
	push byte %1
	jmp isr_stub
%endmacro


%macro ISR_ERROR_CODE 1
[global isr%1]
isr%1:
	cli
	push byte %1
	jmp isr_stub
%endmacro

%macro IRQ 2
[global irq%1]
irq%1:
	cli
	push byte 0
	push byte %2
	jmp irq_stub
%endmacro

;; 8 and 10-14 have inclusive error code, more in intel manual
ISR_NO_ERROR_CODE 0
ISR_NO_ERROR_CODE 1
ISR_NO_ERROR_CODE 2
ISR_NO_ERROR_CODE 3
ISR_NO_ERROR_CODE 4
ISR_NO_ERROR_CODE 5
ISR_NO_ERROR_CODE 6
ISR_NO_ERROR_CODE 7
ISR_ERROR_CODE 8
ISR_NO_ERROR_CODE 9
ISR_ERROR_CODE 10
ISR_ERROR_CODE 11
ISR_ERROR_CODE 12
ISR_ERROR_CODE 13
ISR_ERROR_CODE 14
ISR_NO_ERROR_CODE 15
ISR_NO_ERROR_CODE 16
ISR_ERROR_CODE 17
ISR_NO_ERROR_CODE 18
ISR_NO_ERROR_CODE 19
ISR_NO_ERROR_CODE 20
ISR_NO_ERROR_CODE 21
ISR_NO_ERROR_CODE 22
ISR_NO_ERROR_CODE 23
ISR_NO_ERROR_CODE 24
ISR_NO_ERROR_CODE 25
ISR_NO_ERROR_CODE 26
ISR_NO_ERROR_CODE 27
ISR_NO_ERROR_CODE 28
ISR_NO_ERROR_CODE 29
ISR_ERROR_CODE 30
ISR_NO_ERROR_CODE 31
IRQ	0, 32
IRQ	1, 33
IRQ	2, 34
IRQ	3, 35
IRQ	4, 36
IRQ	5, 37
IRQ	6, 38
IRQ	7, 39
IRQ	8, 40
IRQ	9, 41
IRQ	10, 42
IRQ	11, 43
IRQ	12, 44
IRQ	13, 45
IRQ	14, 46
IRQ	15, 47 

[EXTERN isr_handler]
[EXTERN irq_handler]

isr_stub:
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

irq_stub: 
	pusha
	
	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call	irq_handler

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa
	add esp, 8
	sti
	iret


