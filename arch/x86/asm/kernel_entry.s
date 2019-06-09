[BITS 32]
[extern kmain]

;; --------------------------------------
;; kernel_entry.s - kernel start proc
;; System sources under license MIT
;; --------------------------------------

	call	kmain
	jmp	$
