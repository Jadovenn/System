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

add_dependencies(cpuid Library::c_abi)
target_link_libraries(cpuid PUBLIC Library::c_abi)
target_include_directories(cpuid
        PUBLIC
            ${c_abi_INCLUDE_DIRS}
            include)

############################
## Exported variables     ##
############################

add_library(Library::cpuid ALIAS cpuid)
set(cpuid_FOUND True CACHE INTERNAL "")
set(cpuid_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" FORCE)
