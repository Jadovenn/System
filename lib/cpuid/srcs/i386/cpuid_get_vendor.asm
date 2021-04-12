;; ------------------------------------
;; cpuid_get_vendor.asm - get cpu vendor
;; System sources under license MIT
;; ------------------------------------

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

zhaoxin:        db "  Shanghai  "
                dd 12

hygon:          db "HygonGenuine"
                dd 13

elbrus:         db "E2K MACHINE "
                dd 14


;; Virtual machine CPU
vmware:         db "VMwareVMware"
                dd 15

xen:            db "XenVMMXenVMM"
                dd 16

microsoft_hv:   db "Microsoft Hv"
                dd 17

parallels:       db " lrpepyh vr "
                dd 18

qemu:           db "TCGTCGTCGTCG"
                dd 19

bhyve:          db "bhyve bhyve "
                dd 20

kvm:            db " KVMKVMKVM  "
                dd 21

acrn:           db "ACRNACRNACRN"
                dd 22

qnx:            db " QNXQVMBSQG "
                dd 23

apple_rosetta:  db "VirtualApple"
                dd 24

vendor_name: dd old_amd, amd, intel, centaur, old_transmeta, transmeta, cyrix, \
                centaur, nexgen, umc, sis, nsc, rise, vortex, via, zhaoxin, hygon, elbrus, \
                vmware, xen, microsoft_hv, parallels, bhyve, kvm, acrn, qnx, apple_rosetta, 0

section .text

;; Cpuid_processor_vendor_t STDCALL cpuid_get_vendor();

global cpuid_get_vendor:function (cpuid_get_vendor.end - cpuid_get_vendor)
cpuid_get_vendor:
    push ebp
    mov  ebp, esp
    push ebx
    push ecx

    mov eax, 0x0
    cpuid

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
