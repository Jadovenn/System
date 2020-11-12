##
## build.config.cmake - Build configuration
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(System
		VERSION 1.0
		DESCRIPTION "Kernel"
		LANGUAGES C)

##set(CMAKE_VERBOSE_MAKEFILE ON)

include(build.config.cmake)
include(tools/cmake/build-header.cmake)

if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR " -- You must specify a toolchain configuration file, please read the documentation")
endif(NOT CMAKE_TOOLCHAIN_FILE)

include(tools/cmake/load-dependencies.cmake)

set(kernel_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/kernel/include)

add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
add_compile_options("$<$<CONFIG:DEBUG>:-g>")
load_dependencies(lib)
add_subdirectory(arch/${ARCH})

add_subdirectory(kernel)
include(tools/cmake/iso-generation.cmake)
include(tools/cmake/qemu-run.cmake)
