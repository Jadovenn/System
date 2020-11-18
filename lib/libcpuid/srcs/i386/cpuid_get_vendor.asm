[BITS 32]

[GLOBAL cpuid_get_vendor]

section .data

;; vendor data layout

old_amd:        db "AMDisbetter!"
                dd 0

amd:            db "AuthenticAMD"
                dd 0

intel:          db "GenuineIntel"
                dd 1

old_transmeta:  db "TransmetaCPU"
                dd 2

transmeta:      db "GenuineTMx86"
                dd 2

cyrix:          db "CyrixInstead"
                dd 3

centaur:        db "CentaurHauls"
                dd 4

nexgen:         db "NexGenDriven"
                dd 5

umc:            db "UMC UMC UMC "
                dd 6

sis:            db "SiS SiS SiS "
                dd 7

nsc:            db "Geode by NSC"
                dd 7

rise:           db "RiseRiseRise"
                dd 8

vortex:         db "Vortex86 SoC"
                dd 9

via:            db "VIA VIA VIA "
                dd 11

vmware:         db "VMwareVMware"
                dd 12

xen:            db "XenVMMXenVMM"
                dd 13

microsoft_hv:   db "Microsoft Hv"
                dd 14

parallel:       db " lrpepyh vr "
                dd 15

vendor_name: dd old_amd, \
                amd, \
                intel, \
                centaur, \
                old_transmeta, \
                transmeta, \
                cyrix, \
                centaur, \
                nexgen, \
                umc, \
                sis, \
                nsc, \
                rise, \
                vortex, \
                via, \
                vmware, \
                xen, \
                microsoft_hv, \
                parallel, \
                0

cpuid_max_cap: db 0x0

section .text

global cpuid_get_vendor:function (cpuid_get_vendor.end - cpuid_get_vendor)
cpuid_get_vendor:
    push ebp
    mov  ebp, esp
    push ebx
    push ecx

    mov eax, 0x0
    cpuid
    mov [cpuid_max_cap], eax

    xor esi, esi

.loop:
    mov eax, [vendor_name + esi]

    cmp eax, 0
    je  cpuid_get_vendor.exit_error
    mov edi, DWORD[eax]
    cmp edi, 0
    cmp edi, ebx
    jne cpuid_get_vendor.loop_continue

    mov edi, DWORD[eax + 4]
    cmp edi, edx
    jne cpuid_get_vendor.loop_continue

    mov edi, DWORD[eax + 8]
    cmp edi, ecx
    jne cpuid_get_vendor.loop_continue

    mov eax, DWORD[eax + 12]
    jmp cpuid_get_vendor.exit_success

.loop_continue:
    add esi, 4
    jmp cpuid_get_vendor.loop

.exit_error:
    mov eax, -1
.exit_success:
    pop ecx
    pop ebx
    mov esp, ebp
    pop ebp
    ret
.end:
