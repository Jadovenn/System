;; ------------------------------------
;; cpuid_get_capabilities.asm - get cpu capabilities
;; System sources under license MIT
;; ------------------------------------

[BITS 32]
[GLOBAL cpuid_get_features]

section .text

;; Cpuid_capabilities_t CDECL cpuid_get_capabilities();

global cpuid_get_capabilities:function (cpuid_get_capabilities.end - cpuid_get_capabilities)
cpuid_get_capabilities:
    push ebp
    mov  ebp, esp

    mov  eax, 1
    cpuid
    mov eax, [ebp + 8]
    mov [eax], ecx
    mov [eax + 4], edx

    mov esp, ebp
    pop ebp
    ret
.end:
