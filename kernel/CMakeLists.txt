##
## kernel/CMakeLists.txt - Kernel project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(Kernel
        VERSION 1.0
        DESCRIPTION "Kernel"
        LANGUAGES C)

####################################################
## Export cmake's variable for the kernel project ##
####################################################

set(kernel_FOUND True CACHE INTERNAL "")
set(kernel_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" FORCE)

###################################################
## Include the corresponding architecture loader ##
###################################################

add_subdirectory(arch/${ARCH})

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
        srcs/io/console.c
        srcs/io/printf.c
        srcs/memory/malloc.c
        srcs/main.c)

###########################
## Kernel binary target  ##
###########################

add_executable(${KERNEL_NAME} $<TARGET_OBJECTS:c_RUNTIME> ${SRCS})

add_dependencies(${KERNEL_NAME}
        Kernel::arch
        Library::c_abi
        Library::system
        Library::cpuid)

target_include_directories(${KERNEL_NAME}
        PUBLIC
            ${c_abi_INCLUDE_DIRS}
            ${system_INCLUDE_DIRS}
            ${cpuid_INCLUDE_DIRS}
        PRIVATE
            ${kernel_INCLUDE_DIRS})

target_link_libraries(${KERNEL_NAME}
        PRIVATE
            Kernel::arch
            Library::c_abi
            Library::cpuid
            gcc)

target_link_options(${KERNEL_NAME}
        PRIVATE
        -T${loader_LINKER_SCRIPT})

set_target_properties(${KERNEL_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../sysroot/boot")
