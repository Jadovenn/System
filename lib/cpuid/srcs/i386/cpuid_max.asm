;; ------------------------------------
;; cpuid_max.asm - get max cpuid eax value
;; System sources under license MIT
;; ------------------------------------

[BITS 32]
[GLOBAL cpuid_max]

section .text

;; int STDCALL cpuid_max();

global cpuid_max:function (cpuid_max.end - cpuid_max)
cpuid_max:
    push ebp
    mov  ebp, esp
    push ebx
    push ecx

    xor  eax, eax
    cpuid

    pop ecx
    pop ebx
    mov esp, ebp
    pop ebp
    ret
.end:
