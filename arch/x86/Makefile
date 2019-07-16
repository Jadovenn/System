##
## x86/Makefile - specific i386 makefile
## System sources under license MIT
##

## Constante
VERSION		=	v0.0.1
KERNEL		=	system
KERNEL_BOOT	=	system-$(VERSION)-with-bootloader
SYSTEM_IMG	=	system-$(VERSION)-bare_bones-with-bootloader.img
SYSTEM_ISO	=	system-$(VERSION)-bare_bones.iso
SYSTEM_SYM	=	system-$(VERSION)-sym.elf
SYSTEM_SYM_MLTB	=	system-$(VERSION)-sym-mltb.elf
BOOTLOADER	=	bootloader.bin
MULTIBOOT	=	mtboot.o
ROOT		=	../..

## Bootloader for system-boot, now deprecated, source are still here but not used
BOOTLOADER_SRC	=	boot/bootloader.s

## Multiboot header for system multiboot
MULTIBOOT_SRC	=	boot/multiboot.s

## System asm sources
KERNEL_ASM_SRC	=	asm/interrupt.s \
			asm/dt_flush.s

## System C sources
KERNEL_C_SRC	=	kernel/kernel.c \
			kernel/timer.c \
			kernel/io/ports.c \
			kernel/io/printk.c \
			kernel/logs/log.c \
			kernel/init/monitor_init.c \
			kernel/init/descriptor_tables_init.c \
			drivers/monitor/monitor.c \
			cpu/pic.c \
			cpu/isr.c

## Include directories
HEADERS		=	-Iinclude \
			-I$(ROOT)/lib/libc/include \
			-I$(ROOT)/include

## Kernel space libc sources
LIBC_SRC	=	$(ROOT)/lib/libc/memory.c

## System OBJ
KERNEL_ASM_OBJ	=	$(KERNEL_ASM_SRC:.s=.o)
KERNEL_C_OBJ	=	$(KERNEL_C_SRC:.c=.o)
LIBC_OBJ	=	$(LIBC_SRC:.c=.o)

CFLAGS		:=	$(CFLAGS) \
			$(HEADERS) \
			-Wall \
			-Wextra \
			-nostdlib \
			-nostdinc \
			-fno-builtin \
			-ffreestanding \
			-g

LDFLAGS		+=	-Tlink.ld

## Select the right toolchains or cross-compiler
## for this architecture depending the host operating system
UNAME		:=	$(shell uname)
ifeq ($(UNAME), Darwin)
	CC	=	i386-elf-gcc
	LD	=	i386-elf-ld
	NASM	=	nasm
endif
ifeq ($(UNAME), Linux)
	CFLAGS	+=	-fno-pie -fno-stack-protector
	CC	=	gcc -m32 
	LD	=	ld -melf_i386
	NASM	=	nasm
endif

## Top level rules
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
$(KERNEL): $(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ)

$(MULTIBOOT):
	nasm -felf32 $(MULTIBOOT_SRC) -o $(MULTIBOOT)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.s.o:
	$(NASM) $< -f elf -o $@

$(SYSTEM_ISO):	$(KERNEL)
	script/build-iso.sh $(SYSTEM_ISO)

$(SYSTEM_SYM):	$(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ)
	$(LD) $(LDFLAGS) -o $(SYSTEM_SYM_MLTB) $(MULTIBOOT) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ)

.PHONY:	all standalone iso clean run run-mltb debug debug-mltb

