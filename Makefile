##
## Makefile - top level makefile
## System sources under license MIT
##

##################################################
##                 CONSTANTS                    ##
##################################################

VERSION		=	v0.0.2
KERNEL		=	system
KERNEL_BOOT	=	system-$(VERSION)-with-bootloader
SYSTEM_IMG	=	system-$(VERSION)-bare_bones-with-bootloader.img
SYSTEM_ISO	=	system-$(VERSION)-bare_bones.iso
SYSTEM_SYM	=	system-$(VERSION)-sym.elf
SYSTEM_SYM_MLTB	=	system-$(VERSION)-sym-mltb.elf
BOOTLOADER	=	bootloader.bin
MULTIBOOT	=	mtboot.o
ROOT		=	../..

ARCH	=	x86
TARGET	=	arch/$(ARCH)
MODE	=	release

LIBC_PATH	=	lib/libc
LIBC		=	$(LIBC_PATH)/libc.a

KERNEL_C_SRC	=	kernel/printk.c \
			kernel/heap.c

HEADERS		=	-Iinclude \
			-Ilib/libc/include

##################################################
##                   MODES                      ##
##################################################

ifeq ($(MODE), release) ## RELEASE

KERNEL_C_SRC	+=	kernel/main.c

CFLAGS		+=	-O3

endif ## END RELEASE
ifeq ($(MODE), debug) ## DEBUG

KERNEL_C_SRC	+=	kernel/main.c

CFLAGS		+=	-g

endif ## END DEBUG
ifeq ($(MODE), test) ## TEST

KERNEL_C_SRC	+=	test/main.c \
			test/memccpy.c
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

BOOTLOADER_SRC	=	$(TARGET)/boot/bootloader.s

## Multiboot header for system multiboot
MULTIBOOT_SRC	=	$(TARGET)/boot/multiboot.s

## System asm sources
KERNEL_ASM_SRC	=	$(TARGET)/asm/interrupt.s \
			$(TARGET)/asm/dt_flush.s

KERNEL_C_SRC	+=	$(TARGET)/kernel/kernel.c \
			$(TARGET)/kernel/timer.c \
			$(TARGET)/kernel/io/ports.c \
			$(TARGET)/kernel/logs/log.c \
			$(TARGET)/kernel/init/gdt_init.c \
			$(TARGET)/kernel/init/idt_init.c \
			$(TARGET)/kernel/init/mmu_init.c \
			$(TARGET)/kernel/init/monitor_init.c \
			$(TARGET)/cpu/pic.c \
			$(TARGET)/cpu/isr.c \
			$(TARGET)/cpu/cr.c \
			$(TARGET)/cpu/mmu/mmu.c \
			$(TARGET)/cpu/mmu/page.c \
			$(TARGET)/cpu/mmu/frame.c \
			$(TARGET)/drivers/monitor/monitor.c

HEADERS		+=	-I$(TARGET)/include

endif ## END x86

##################################################
##                  OBJECTS                     ##
##################################################

KERNEL_ASM_OBJ	=	$(KERNEL_ASM_SRC:.s=.o)
KERNEL_C_OBJ	=	$(KERNEL_C_SRC:.c=.o)

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

clean:
	rm -f	$(KERNEL)
	rm -f	$(KERNEL_BOOT)
	rm -f	$(SYSTEM_IMG)
	rm -f	$(SYSTEM_ISO)
	rm -f	$(SYSTEM_SYM)
	rm -f	$(SYSTEM_SYM_MLTB)
	rm -f	$(BOOTLOADER)
	rm -f	$(MULTIBOOT)
	rm -f	$(KERNEL_C_OBJ)
	rm -f	$(KERNEL_ASM_OBJ)
	rm -f	$(LIBC_OBJ)
	rm -f   peda-*

re:		clean all

run:		$(SYSTEM_ISO)
	qemu-system-i386 -cdrom $(SYSTEM_ISO)

debug:	$(SYSTEM_ISO) $(SYSTEM_SYM)
	qemu-system-i386 -s -S -fda $(SYSTEM_ISO) &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(SYSTEM_SYM_MLTB)"

## Generation rules
$(KERNEL): $(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ)
	$(LD) -o $(KERNEL) $(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LDFLAGS)

$(MULTIBOOT):
	nasm -felf32 $(MULTIBOOT_SRC) -o $(MULTIBOOT)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.s.o:
	$(NASM) $< -f elf -o $@

$(SYSTEM_ISO):	$(KERNEL)
	scripts/build-iso.sh $(SYSTEM_ISO)

$(SYSTEM_SYM):	$(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ)
	$(LD) -o $(SYSTEM_SYM_MLTB) $(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ) $(LDFLAGS)

.PHONY:	all standalone iso clean run run-mltb debug debug-mltb

