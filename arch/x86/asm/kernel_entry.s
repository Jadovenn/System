[BITS 32]
[EXTERN kernel_main]
[EXTERN _init]

;; --------------------------------------
;; kernel_entry.s - kernel start proc
;; System sources under license MIT
;; --------------------------------------

	call	_init
	call	kernel_main
	jmp	$

