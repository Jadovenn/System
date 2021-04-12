##
## i386/linux-toolchain.cmake - linux toolchain config file
## System sources under license MIT
##

## Use this toolchain config file with gcc multilib installed

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i386)
set(UNIX True CACHE INTERNAL "" FORCE)

set(CMAKE_CROSSCOMPILING TRUE)

## C compiler
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS
" -m32 \
-march=i386 \
-Wall \
-fno-pie \
-fno-stack-protector \
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

## NASM
set(CMAKE_NASM nasm)

set(CMAKE_AR ar)
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> rcs <TARGET> <LINK_FLAGS> <OBJECTS>")

## Other tools
set(CMAKE_OBJCOPY objcopy)
set(CMAKE_NM nm)
set(CMAKE_GDB gdb)
set(CMAKE_AWK awk)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
