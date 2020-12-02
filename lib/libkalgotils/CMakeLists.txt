##
## lib/libc/CMakeList.txt - libc project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(libkalgotils
        VERSION 1.0
        DESCRIPTION "Kernel utilities and algorithms library"
        LANGUAGES C)

################
## Sources    ##
################

set(HEADERS
        include/algo/list.h
        include/utils/container_of.h)

set(SRCS
        srcs/list.c)

################
## Targets    ##
################

add_library(kalgotils ${SRCS} ${HEADERS})
add_dependencies(kalgotils Dependencies::libc)
target_link_libraries(kalgotils
        PUBLIC
        Dependencies::libc)
target_include_directories(kalgotils
        PUBLIC
        include
        ${libc_INCLUDE_DIRS}
        ${kernel_INCLUDE_DIRS})
add_library(Dependencies::libkalgotils ALIAS kalgotils)

############################
## Exported variables     ##
############################

set(libkalgotils_FOUND True CACHE INTERNAL "")
set(libkalgotils_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" FORCE)
