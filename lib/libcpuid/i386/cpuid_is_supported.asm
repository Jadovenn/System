[BITS 32]

[GLOBAL cpuid_is_supported]

section .text
global cpuid_is_supported:function (cpuid_is_supported.end - cpuid_is_supported)
cpuid_is_supported:
    pushfd
    pushfd
    xor dword [esp], 0x00200000
    popfd
    pushfd
    pop eax
    xor eax, [esp]
    popfd
    and eax, 0x00200000
    ret
.end:
