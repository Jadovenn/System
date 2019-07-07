[BITS 32]
[extern kernel_main]

;; --------------------------------------
;; kernel_entry.s - kernel start proc
;; System sources under license MIT
;; --------------------------------------

	call	kernel_main
	jmp	$

