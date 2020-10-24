##
## dependency.mk - libc dependency config
## System sources under license MIT
##

DEPENDENCIES_DIR		+= lib/libc

DEPENDENCIES_HEADERS	+= -Ilib/libc/include

DEPENDENCIES_TARGET		+= lib/libc/libc.a

DEPENDENCIES_LD_FLAGS	+= -Llib/libc -lc
