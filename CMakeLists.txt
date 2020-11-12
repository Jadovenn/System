##
## CMakeLists.txt - System project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(System
		VERSION 1.0
		DESCRIPTION "Hobby Kernel project"
		LANGUAGES C)

##set(CMAKE_VERBOSE_MAKEFILE ON)

############################
## Project config         ##
############################

include(build.config.cmake)
include(tools/cmake/build-header.cmake)

## If no toolchain manually provided then cmake fail
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR " -- You must specify a toolchain configuration file, please read the documentation")
endif(NOT CMAKE_TOOLCHAIN_FILE)

add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
add_compile_options("$<$<CONFIG:DEBUG>:-g>")

############################
## Project sources        ##
############################

include(tools/cmake/load-dependencies.cmake)
set(kernel_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/kernel/include)

load_dependencies(lib)
add_subdirectory(arch/${ARCH})
add_subdirectory(kernel)

############################
## Run target             ##
############################

include(tools/cmake/iso-generation.cmake)
include(tools/cmake/qemu-run.cmake)
