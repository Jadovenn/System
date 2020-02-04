##
## Makefile - top level makefile
## System sources under license MIT
##

##################################################
##               CONFIGURATION                  ##
##################################################

include build.config

##################################################
##                 CONSTANTS                    ##
##################################################

NAME		=	system
KERNEL		=	$(NAME).kern
SYSTEM_ISO	=	$(NAME)_v$(VERSION)-$(RELEASE_NAME).iso

TARGET	=	arch/$(ARCH)

LIBC_PATH	=	lib/libc
LIBC		=	$(LIBC_PATH)/libc.a

##################################################
##                  SOURCES                     ##
##################################################

include kernel/make.config

COMMON_HEADERS	=	$(addprefix -I, $(KERNEL_INCLUDE_DIR)) \
			-Ilib/libc/include

include  test/make.config

ARCH_DIR	=	arch/$(ARCH)

##################################################
##                   MODES                      ##
##################################################

SRCS		+=	$(KERNEL_SRCS)
HEADERS		+=	$(COMMON_HEADERS)

ifeq ($(MODE), release) ## RELEASE

SRCS		+=	$(KERNEL_ENTRY_POINT)

CFLAGS		+=	-O3

endif ## END RELEASE
ifeq ($(MODE), debug) ## DEBUG

SRCS		+=	$(KERNEL_ENTRY_POINT)

CFLAGS		+=	-g

endif ## END DEBUG
ifeq ($(MODE), test) ## TEST

SRCS		+=	$(TEST_SRCS)
COMMON_HEADERS	+=	$(addprefix -I, $(TEST_HEADERS))
CFLAGS		+=	-g

endif ## END TEST

##################################################
##                   ARCH                       ##
##################################################

UNAME		:=	$(shell uname)

ifeq ($(ARCH), i386) ## i386

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

include	$(ARCH_DIR)/make.config

endif ## END x86

MULTIBOOT	+=	$(KERNEL_ARCH_MULTIBOOT)
ASM		+=	$(KERNEL_ARCH_ASM)
SRCS		+=	$(KERNEL_ARCH_SRCS)
HEADERS		+=	$(addprefix -I, $(KERNEL_ARCH_INCLUDE))
LDFLAGS		+=	$(KERNEL_ARCH_LDFLAGS)

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
	rm -f	$(TEST_SRCS:.c=.o)
	rm -f   peda-*

re:		clean all

dependency-clean:
	$(MAKE) -C lib/libc clean

$(SYSTEM_ISO):	$(KERNEL)
	scripts/build-iso.sh $(SYSTEM_ISO)

run:		$(SYSTEM_ISO)
	$(QEMU) -m $(PHYSICAL_MEM) -fda $(SYSTEM_ISO)

debug:		$(SYSTEM_ISO)
	$(QEMU) -m $(PHYSICAL_MEM) -s -S -fda $(SYSTEM_ISO) &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL)"

help:
	@echo "System Makefile:"
	@echo "\tall: build all"
	@echo "\tiso: build and create bootable iso"
	@echo "\trun: run in qemu"
	@echo "\tdebug: run with gdb"
	@echo "see build.conf for more details"


.PHONY:	all iso .c.o .s.o clean re run debug help

