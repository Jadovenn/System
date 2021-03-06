##
## i386/macos-toolchain.cmake - macos toolchain config
## System sources under license MIT
##

## Use this toolchain file with i386-elf-gcc package installed

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i386)
set(APPLE True CACHE INTERNAL "" FORCE)
set(UNIX True CACHE INTERNAL "" FORCE)

set(CMAKE_CROSSCOMPILING TRUE)

set(BIN_PATH /usr/local/Cellar/i386-elf-gcc/9.2.0/bin)

## C compiler
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER /usr/local/bin/i386-elf-gcc)
set(CMAKE_C_FLAGS
"-Wall \
-Wextra \
-nostdinc \
-nostdlib \
-nostartfiles \
-fno-pic \
-fno-builtin \
-static \
-static-libgcc \
-lgcc \
-ffreestanding")

## C++ compiler
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER ${BIN_PATH}/i386-elf-g++)

## NASM
set(CMAKE_NASM /usr/local/bin/nasm)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> \
    <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_ASM_NASM_FLAGS "${ASM_NASM_FLAGS} -g -F dwarf")
else()
    set(CMAKE_ASM_NASM_FLAGS "${ASM_NASM_FLAGS}")
endif()

set(CMAKE_AR /usr/local/bin/i386-elf-ar)
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> rcs <TARGET> <LINK_FLAGS> <OBJECTS>")

## Other tools
set(CMAKE_OBJCOPY /usr/local/bin/i386-elf-objcopy)
set(CMAKE_NM /usr/local/bin/i386-elf-nm)
set(CMAKE_GDB gdb)
set(CMAKE_AWK awk)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
