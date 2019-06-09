##
##	Makefile
##

KERNEL		=	system
KERNEL_IMG	=	vmsystem.img
KERNEL_SYM	=	kernel.elf
BOOTLOADER	=	bootloader.bin
ROOT		=	../..

BOOTLOADER_SRC	=	boot/bootloader.s

KERNEL_ASM_SRC	=	asm/kernel_entry.s \
			asm/interrupt.s \
			kernel/init/dt_flush.s

KERNEL_C_SRC	=	kernel/kernel.c \
			kernel/timer.c \
			kernel/io/ports.c \
			kernel/io/printk.c \
			kernel/logs/log.c \
			kernel/init/init.c \
			kernel/init/monitor_init.c \
			kernel/init/descriptor_tables_init.c \
			kernel/memory/heap.c \
			drivers/monitor/monitor.c \
			cpu/pic.c \
			cpu/isr.c

KERNEL_ASM_OBJ	=	$(KERNEL_ASM_SRC:.s=.o)
KERNEL_C_OBJ	=	$(KERNEL_C_SRC:.c=.o)

LIBC_SRC	=	$(ROOT)/lib/libc/memory.c

LIBC_OBJ	=	$(LIBC_SRC:.c=.o)

UNAME		:=	$(shell uname)

CFLAGS		=	-Iinclude \
			-I$(ROOT)/lib/libc/include \
			-I$(ROOT)/include

LDFLAGS		=	-Tlink.ld

ifeq ($(UNAME), Darwin)
	CC	=	i386-elf-gcc -g -nostdinc -nostdlib -fno-builtin
	LD	=	i386-elf-ld
	NASM	=	nasm
endif
ifeq ($(UNAME), Linux)
	CC	=	gcc -m32 -fno-pie -fno-stack-protector
	LD	=	ld -melf_i386
	NASM	=	nasm
endif

all:	$(KERNEL_IMG)

$(KERNEL_IMG):	$(BOOTLOADER) $(KERNEL)
	cat $(BOOTLOADER) $(KERNEL) > $(KERNEL_IMG)

run:	all qemue

$(BOOTLOADER):
	nasm -f bin $(BOOTLOADER_SRC) -o $(BOOTLOADER)

$(KERNEL):	$(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ) --oformat binary

.c.o:
	$(CC) $(CFLAGS) -ffreestanding -c $< -o $@
.s.o:
	$(NASM) $< -f elf -o $@
clean:
	rm -f	$(KERNEL_IMG)
	rm -f	$(KERNEL)
	rm -f	$(KERNEL_SYM)
	rm -f	$(BOOTLOADER)
	rm -f	$(KERNEL_C_OBJ)
	rm -f	$(KERNEL_ASM_OBJ)
	rm -f	$(LIBC_OBJ)
run:
	qemu-system-i386 -fda $(KERNEL_IMG)

kernel_symbole: all
	$(LD) $(LDFLAGS) -o $(KERNEL_SYM) -Ttext 0x1000 $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LIBC_OBJ)

debug:	kernel_symbole
	qemu-system-i386 -s -S -fda $(KERNEL_IMG) &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

re:	clean all

.PHONY:	all clean run qemue qemue-debug kernel_symbole

