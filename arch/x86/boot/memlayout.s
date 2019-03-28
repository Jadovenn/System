[BITS 32]
[BITS 32]
[EXTERN entrypgdir]
;; -------------------------------------------
;; memlayout.s - kernel memory layout
;; System source under license MIT
;; -------------------------------------------

%define	CR4_PSE	0x00000010
%define CR0_WP	0x00010000
%define CR0_PG	0x00000010
%define CR0_ENABLE_PAGING	CR0_WP | CR0_PG

%define KERNEL_BASE 0xC0000000

;; Translate Virtual address to physical address
%define V2P(x)	(x - KERNEL_BASE)

