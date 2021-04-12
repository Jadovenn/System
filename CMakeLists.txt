##
## CMakeLists.txt - System project
## System sources under license MIT
##

cmake_minimum_required(VERSION 3.11...3.16)
project(System
		VERSION 1.0
		DESCRIPTION "Hobby Kernel project"
		LANGUAGES C)

## Uncomment this flags to have verbosity on make
##set(CMAKE_VERBOSE_MAKEFILE ON)

####################################
## Project early config and check ##
####################################

## Load user config
include(build.config.cmake)

## Include cmake tools required to build System
include(tools/cmake/build-header.cmake)
include(tools/cmake/load-libraries.cmake)
include(tools/cmake/iso-generation.cmake)
include(tools/cmake/qemu-run.cmake)

## If no toolchain manually provided then cmake generation must fail
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR " -- You must specify a toolchain configuration file, please read the documentation")
endif(NOT CMAKE_TOOLCHAIN_FILE)

## Compile option based on the CMAKE_BUILD_TYPE
add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
add_compile_options("$<$<CONFIG:DEBUG>:-g>")

#####################
## Project targets ##
#####################

## Load libraries that are agnostic from the kernel
add_subdirectory(lib/c_abi)
add_subdirectory(lib/system)
add_subdirectory(lib/cpuid)

## Load the kernel project
add_subdirectory(kernel)