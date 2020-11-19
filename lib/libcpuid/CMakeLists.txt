##
## lib/libc/CMakeList.txt - libc project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(libcpuid
        VERSION 1.0
        DESCRIPTION "CPUID library"
        LANGUAGES C ASM_NASM)

################
## Sources    ##
################

set(HEADERS
        include/cpuid/cpuid.h)

set(SRCS
        srcs/i386/cpuid_is_supported.asm
        srcs/i386/cpuid_max.asm
        srcs/i386/cpuid_get_vendor.asm
        srcs/i386/cpuid_get_model_string.asm
        srcs/i386/cpuid_get_capabilities.asm
        srcs/get_vendor_string.c)

################
## Targets    ##
################

add_library(cpuid ${SRCS} ${HEADERS})
target_link_libraries(cpuid
        PUBLIC
            Dependencies::libc)
target_include_directories(cpuid
        PUBLIC
            include
            ${libc_INCLUDE_DIRS})
add_dependencies(cpuid Dependencies::libcpuid)
add_library(Dependencies::libcpuid ALIAS cpuid)

############################
## Exported variables     ##
############################

set(libcpuid_FOUND True CACHE INTERNAL "")
set(libcpuid_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" FORCE)
