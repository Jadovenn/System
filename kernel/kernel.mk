##
## kernel.mk - config kernel sources
## System sources under license MIT
##

KERNEL_INCLUDE_DIRS	=	kernel/include

KERNEL_ENTRY_POINT	=	kernel/main.c

KERNEL_C_SRCS		=	kernel/io/console.c \
						kernel/io/printf.c \
						kernel/memory/malloc.c \
						kernel/memory/vmalloc.c
