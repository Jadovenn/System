##
## Makefile - top level makefile
## System sources under license MIT
##

##################################################
##                 CONSTANTS                    ##
##################################################

VERSION		=	0.0.3
RELEASE_NAME	=	bare_bones
NAME		=	system
KERNEL		=	$(NAME).kern
SYSTEM_ISO	=	$(NAME)_v$(VERSION)-$(RELEASE_NAME).iso

ARCH	=	x86
TARGET	=	arch/$(ARCH)
MODE	=	release

LIBC_PATH	=	lib/libc
LIBC		=	$(LIBC_PATH)/libc.a

##################################################
##                  SOURCES                     ##
##################################################

COMMON_SRC	=	kernel/printk.c \
			kernel/heap.c

COMMON_HEADERS	=	-Iinclude \
			-Ilib/libc/include

KERNEL_SRCS	=	kernel/main.c

TEST_SRCS	=	test/main.c \
			test/heap.c \
			test/memccpy.c \
			test/strlen.c

X86_PATH	=	arch/x86

## Multiboot header for system multiboot
X86_MULTIBOOT	=	$(X86_PATH)/boot/multiboot.s
## System asm sources
X86_ASM		=	$(X86_PATH)/asm/interrupt.s \
			$(X86_PATH)/asm/dt_flush.s

X86_SRCS	=	$(X86_PATH)/kernel/kernel.c \
			$(X86_PATH)/kernel/timer.c \
			$(X86_PATH)/kernel/io/ports.c \
			$(X86_PATH)/kernel/logs/log.c \
			$(X86_PATH)/kernel/init/gdt_init.c \
			$(X86_PATH)/kernel/init/idt_init.c \
			$(X86_PATH)/kernel/init/mmu_init.c \
			$(X86_PATH)/kernel/init/monitor_init.c \
			$(X86_PATH)/cpu/pic.c \
			$(X86_PATH)/cpu/isr.c \
			$(X86_PATH)/cpu/cr.c \
			$(X86_PATH)/cpu/mmu/mmu.c \
			$(X86_PATH)/cpu/mmu/page.c \
			$(X86_PATH)/cpu/mmu/frame.c \
			$(X86_PATH)/drivers/monitor/monitor.c

##################################################
##                   MODES                      ##
##################################################

SRCS		+=	$(COMMON_SRC)
HEADERS		+=	$(COMMON_HEADERS)

ifeq ($(MODE), release) ## RELEASE

SRCS		+=	$(KERNEL_SRCS)

CFLAGS		+=	-O3

endif ## END RELEASE
ifeq ($(MODE), debug) ## DEBUG

SRCS		+=	$(KERNEL_SRCS)

CFLAGS		+=	-g

endif ## END DEBUG
ifeq ($(MODE), test) ## TEST

SRCS		+=	$(TEST_SRCS)
CFLAGS		+=	-g

endif ## END TEST

##################################################
##                   ARCH                       ##
##################################################

UNAME		:=	$(shell uname)

ifeq ($(ARCH), x86) ## x86

LDFLAGS		+=	-Wl,-Tld/link_x86.ld

ifeq ($(UNAME), Darwin) ## DARWIN
	CC	=	i386-elf-gcc
	LD	=	$(CC)
	NASM	=	nasm
endif ## END DARWIN
ifeq ($(UNAME), Linux) ## LINUX BASED
	CFLAGS	+=	-fno-pie -fno-stack-protector
	CC	=	gcc -m32 
	LD	=	$(CC)
	NASM	=	nasm
endif # END LINUX BASED

MULTIBOOT	+=	$(X86_MULTIBOOT)
ASM		+=	$(X86_ASM)
SRCS		+=	$(X86_SRCS)
HEADERS		+=	-I$(X86_PATH)/include

endif ## END x86

##################################################
##                  OBJECTS                     ##
##################################################

OBJS		+=	$(MULTIBOOT:.s=.o)
OBJS		+=	$(ASM:.s=.o)
OBJS		+=	$(SRCS:.c=.o)

##################################################
##                   FLAGS                      ##
##################################################

CFLAGS		:=	$(CFLAGS) \
			$(HEADERS) \
			-Wall \
			-Wextra \
			-nostdinc \
			-nostartfiles \
			-fno-pic \
			-fno-builtin \
			-ffreestanding

LDFLAGS		+=	-nostdlib \
			-static-libgcc \
			-static \
			-Wl,-static \
			-Wl,-lgcc \
			-Wl,-L$(LIBC_PATH) \
			-Wl,-lc

##################################################
##                  RULES                       ##
##################################################

all:		$(KERNEL)

iso:		$(SYSTEM_ISO)

$(KERNEL):	$(LIBC) $(OBJS)
	$(LD) -o $(KERNEL) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.s.o:
	$(NASM) $< -f elf -o $@

$(LIBC):
	$(MAKE) -C lib/libc

clean:		dependency-clean
	rm -f	$(KERNEL)
	rm -f	$(SYSTEM_ISO)
	rm -f	$(OBJS)
	rm -f   peda-*

re:		clean all

dependency-clean:
	$(MAKE) -C lib/libc clean

$(SYSTEM_ISO):
	scripts/build-iso.sh $(SYSTEM_ISO)

run:		$(SYSTEM_ISO)
	qemu-system-i386 -m 1G -cdrom $(SYSTEM_ISO)

debug:		$(SYSTEM_ISO)
	qemu-system-i386 -m 1G -s -S -fda $(SYSTEM_ISO) &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL)"

.PHONY:	all iso .c.o .s.o clean re run debug 

