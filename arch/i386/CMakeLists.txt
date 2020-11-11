cmake_minimum_required(VERSION 3.11...3.16)
project(libi386
        VERSION 1.0
        DESCRIPTION "Minimal hardware abstract layer."
        LANGUAGES C ASM_NASM)

SET(SOURCES
        multiboot/multiboot.s
        i386_entry.c
        cpu/asm/dt_flush.s
        cpu/asm/interrupt.s
        cpu/pic/pic.c
        cpu/pic/ports.c
        cpu/cr.c
        cpu/dump_regs.c
        cpu/gdt.c
        cpu/idt.c
        cpu/isr.c
        memory/physical.h
        memory/virtual.h
        memory/memory_init.c
        memory/memory_layout.c
        memory/boot/boot_memory.h
        memory/boot/boot_paging.c
        memory/physical/physical_memory_init.c
        memory/physical/physical_memory.c
        memory/virtual/virtual_memory_init.c
        memory/virtual/heap.c
        memory/virtual/mmap.c
        memory/virtual/page_fault_handler.c
        memory/virtual/paging.c
        drivers/monitor/init.c
        drivers/monitor/monitor.h
        drivers/monitor/monitor.c
        drivers/timer_example/timer.c)

add_library(i386 ${SOURCES})
add_dependencies(i386 Dependencies::libc)
target_include_directories(i386
        PUBLIC
            ${kernel_INCLUDE_DIRS}
            ${libc_INCLUDE_DIRS}
        PRIVATE
            include
            ${CMAKE_CURRENT_SOURCE_DIR})

set(arch_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/generic.ld CACHE INTERNAL "")
message("ici: ${arch_LINKER_SCRIPT}")
add_library(Kernel::hal ALIAS i386)
