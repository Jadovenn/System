[BITS 32]
[GLOBAL mboot]
[GLOBAL _start]
;; provided by the linker, see linker script
[EXTERN _kernel_start]
[EXTERN code]
[EXTERN edata]
[EXTERN end]
;; symbole from the kernel
[EXTERN kmain]
[EXTERN _init]
;; ------------------------------------
;; multiboot.s - multiboot kernel entry point
;; System sources under license MIT
;; ------------------------------------

%include "./boot/memlayout.s"

;; Constant
MBALIGN  equ	1 << 0
MEMINFO	 equ	1 << 1
FLAGS	 equ	MBALIGN | MEMINFO
MAGIC	 equ	0x1BADB002
CHECKSUM equ	-(MAGIC + FLAGS)

;; Multiboot header
section	.multiboot
align 4
mboot:
	dd	MAGIC
	dd	FLAGS
	dd	CHECKSUM
	dd	mboot
	dd	code
	dd	edata
	dd	end
	dd	V2P(_start)

;; boot page directory and table, 4KiB each
section .bss
align	4096
boot_page_directory:
resb	0x1000
boot_page_table:
resb	0x1000

;; Stack 16Kib
align	16
stack_bottom:
resb	0x4000
stack_top:


section .text
global _start:function (_start.end - _start)
_start:
	mov	edi, V2P(boot_page_table)
	mov	esi, 0x00
	mov	ecx, 0x3FF
	;; Map the kernel right now
	;; TODO: messy, improve this
.kernel_maping:
	;; while we are below the kernel inc esi
	cmp	esi, V2P(_kernel_start)
	jl	.inc_esi
	;; if we are after the kernel, go to vga mem mapping
	cmp	esi, V2P(end)
	jge	.vga_mem_maping
	mov	edx, esi
	or	edx, 0x003
	mov	[edi], edx

.inc_esi:
	add	esi, 0x1000
	add	edi, 0x4
	loop	.kernel_maping
	
.vga_mem_maping:
	;; map vga memory to 0xC03FF000, present and writable
	mov	DWORD [V2P(boot_page_table) + 0x3FF * 0x4], 0x000B8000 | 0x003

	;; Map page table for virtual address 0x00000000
	mov	DWORD [V2P(boot_page_directory)], V2P(boot_page_table) + 0x003
	;; Map page table for virtual adress 0xC0000000
	mov	DWORD [V2P(boot_page_directory) + 768 * 4], V2P(boot_page_table) + 0x003 

	;; set page size up to 4MiB
	;; mov	ecx, cr4
	;; or	ecx, CR4_PSE
	;; mov	cr4, ecx
	;; Set page directory
	mov	ecx, V2P(boot_page_directory) 
	mov	cr3, ecx
	;; enable paging
	mov	ecx, cr0
	or	ecx, 0x80010000
	mov	cr0, ecx
	;; even after paging enabled, PC is still related to physical address
	;; that why we map this page to both 0x00... and 0xC00..., we must manualy jump to the higher half
	;; and avoid page fault
	mov	ecx, _start.higher_half_addr
	jmp	ecx

.higher_half_addr:
	;; After this label, paging should be enable, and virtual address effective
	mov	DWORD [boot_page_directory], 0
	;; reload cr3 to force TLB flush, change take effect
	invlpg	[0]
	;;mov	ecx, boot_page_directory
	;;mov	cr3, ecx

	mov	esp, stack_top
	;;add	ebx, 0xC0000000
	push	eax
	push	ebx
	;;call	_init
	call	kmain
	cli
.hang:	hlt
	jmp .hang
.end:

