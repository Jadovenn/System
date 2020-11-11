cmake_minimum_required(VERSION 3.11...3.16)
project(System
		VERSION 1.0
		DESCRIPTION "Kernel"
		LANGUAGES C)

set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR " -- You must specify a toolchain configuration file, please read the documentation")
endif(NOT CMAKE_TOOLCHAIN_FILE)



include(tools/cmake/load-dependencies.cmake)
include(build.config.cmake)

SET(kernel_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/kernel/include)

load_dependencies(lib)
add_subdirectory(arch/${ARCH})
add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
##add_compile_options("$<$<CONFIG:DEBUG>:-g>")

message("ici: ${arch_LINKER_SCRIPT}")
set(CMAKE_EXE_LINKER_FLAGS
"-nostartfiles \
-static-libgcc \
-lgcc \
-static \
-Wl,-T${arch_LINKER_SCRIPT} \
-Wl,-static \
-Wl,-lgcc")

SET(KERNEL_ABI_INCLUDE_DIRS
        kernel/include)

add_subdirectory(kernel)
