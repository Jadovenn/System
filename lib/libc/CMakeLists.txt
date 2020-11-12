##
## lib/libc/CMakeList.txt - libc project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(libc
        VERSION 1.0
        DESCRIPTION "C Library"
        LANGUAGES C)

##################
## libc sources ##
##################

SET(SRCS
        include/bitset.h
        include/stdarg.h
        include/stdbool.h
        include/stddef.h
        include/stdint.h
        include/stdlib.h
        include/string.h
        include/unistd.h
        string/memccpy.c
        string/memory.c
        string/string.c
        string/strlen.c
        string/strstr.c
        stdlib/abort.c
        math/div.c)

###############################
## libc target               ##
###############################

add_library(c ${SRCS})
target_include_directories(c PUBLIC include)
add_library(Dependencies::libc ALIAS c)

############################
## Exported variables     ##
############################

set(libc_FOUND True CACHE INTERNAL "")
set(libc_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" FORCE)
