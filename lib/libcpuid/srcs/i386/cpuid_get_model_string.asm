[BITS 32]

[GLOBAL cpuid_get_model_string]
[EXTERN cpuid_max]

section .bss

model_string:
    resb 48

section .text

global cpuid_get_model_string:function (cpuid_get_model_string.end - cpuid_get_model_string)
cpuid_get_model_string:
    push ebp
    mov  ebp, esp
    push ebx
    push ecx
    push edx

    call cpuid_max
    cmp  eax, 0x80000004
    jl   cpuid_get_model_string.exit_failure

    mov eax, 0x80000002
    cpuid
    mov esi, model_string
    mov DWORD[esi], eax
    mov DWORD[esi + 4], ebx
    mov DWORD[esi + 8], ecx
    mov DWORD[esi + 12], edx

    mov eax, 0x80000003
    cpuid
    mov esi, model_string + 16
    mov DWORD[esi], eax
    mov DWORD[esi + 4], ebx
    mov DWORD[esi + 8], ecx
    mov DWORD[esi + 12], edx

    mov eax, 0x80000004
    cpuid
    mov esi, model_string + 32
    mov DWORD[esi], eax
    mov DWORD[esi + 4], ebx
    mov DWORD[esi + 8], ecx
    mov DWORD[esi + 12], edx

    mov eax, model_string
    jmp cpuid_get_model_string.exit_success

.exit_failure:
    mov eax, 0

.exit_success:
    pop edx
    pop ebx
    pop ecx
    mov esp, ebp
    pop ebp
    ret
.end:
