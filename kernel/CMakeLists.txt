cmake_minimum_required(VERSION 3.11...3.16)
project(Kernel
        VERSION 1.0
        DESCRIPTION "Kernel"
        LANGUAGES C)

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

add_executable(${KERNEL_NAME} ${arch_CERT0} ${SRCS})

add_dependencies(${KERNEL_NAME}
        Kernel::hal
        Dependencies::libc)

target_include_directories(${KERNEL_NAME}
        PUBLIC
            ${libc_INCLUDE_DIRS}
        PRIVATE
            ${kernel_INCLUDE_DIRS})

target_link_libraries(${KERNEL_NAME}
        PRIVATE
        Kernel::hal
        gcc
            Dependencies::libc)
target_link_options(${KERNEL_NAME}
        PRIVATE
        -T${arch_LINKER_SCRIPT})

set_target_properties(${KERNEL_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../sysroot/boot")
