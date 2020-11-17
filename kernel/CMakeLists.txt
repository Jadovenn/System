##
## kernel/CMakeLists.txt - Kernel project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(Kernel
        VERSION 1.0
        DESCRIPTION "Kernel"
        LANGUAGES C)
###########################
## Common kernel sources ##
###########################

set(SRCS
        include/hal/console.h
        include/hal/memory.h
        include/kernel/compiler.h
        include/kernel/memory.h
        include/kernel/multiboot.h
        include/kernel/panic.h
        include/kernel/stdio.h
        include/system.h
        io/console.c
        io/printf.c
        memory/malloc.c
        memory/vmalloc.c
        main.c)

###########################
## Kernel binary target  ##
###########################

add_executable(${KERNEL_NAME} ${arch_CERT0} ${SRCS})

add_dependencies(${KERNEL_NAME}
        Kernel::hal
        Dependencies::libc
        Dependencies::libcpuid)

target_include_directories(${KERNEL_NAME}
        PUBLIC
            ${libc_INCLUDE_DIRS}
            ${libcpuid_INCLUDE_DIRS}
        PRIVATE
            ${kernel_INCLUDE_DIRS})

target_link_libraries(${KERNEL_NAME}
        PUBLIC
            Dependencies::libc
            Dependencies::libcpuid
        PRIVATE
            Kernel::hal
            gcc)

target_link_options(${KERNEL_NAME}
        PRIVATE
        -T${arch_LINKER_SCRIPT})

set_target_properties(${KERNEL_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../sysroot/boot")
