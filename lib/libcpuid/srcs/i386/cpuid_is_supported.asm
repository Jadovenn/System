;; ------------------------------------
;; cpuid_is_supported.asm - check cpuid support
;; System sources under license MIT
;; ------------------------------------

[BITS 32]
[GLOBAL cpuid_is_supported]

section .text

;; int STDCALL cpuid_is_supported();

global cpuid_is_supported:function (cpuid_is_supported.end - cpuid_is_supported)
cpuid_is_supported:
    pushfd
    pushfd
    xor DWORD[esp], 0x00200000
    popfd
    pushfd
    pop eax
    xor eax, [esp]
    popfd
    and eax, 0x00200000
    ret
.end:
