##
## lib/libc/CMakeList.txt - libbase project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(libsystem
        VERSION 1.0
        DESCRIPTION "System base"
        LANGUAGES C)

##################
## libc sources ##
##################

SET(SRCS_SYSTEM
        include/system/system.h
        include/system/allocator/allocator.h
        srcs/allocator/allocator.c
        include/system/allocator/pool_allocator.h
        srcs/allocator/pool_allocator.c)

#############
## target  ##
#############

add_library(system ${SRCS_SYSTEM})
add_dependencies(system Library::c_abi)
target_include_directories(system PUBLIC include  ${c_abi_INCLUDE_DIRS})

############################
## Exported variables     ##
############################

add_library(Library::system ALIAS system)
set(system_FOUND True CACHE INTERNAL "")
set(system_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" FORCE)
