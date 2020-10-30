##
## kernel.mk - config kernel sources
## System sources under license MIT
##

KERNEL_INCLUDE_DIRS	=	kernel/include \
						kernel/multiboot

KERNEL_ENTRY_POINT	=	kernel/main.c

KERNEL_C_SRCS		=	kernel/io/printk.c \
						kernel/multiboot/physical_memory.c