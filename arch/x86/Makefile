##
##	Makefile
##

KERNEL		=	netero
KERNEL_IMG	=	vmnetero.img
KERNEL_SYM	=	kernel.elf
BOOTLOADER	=	bootloader.bin

BOOTLOADER_SRC	=	boot/bootloader.s

KERNEL_ASM_SRC	=	kernel/kernel_entry.s
KERNEL_C_SRC	=	kernel/kernel.c \
			kernel/io/ports.c \
			kernel/logs/log.c \
			kernel/init/init.c \
			kernel/init/screen_init.c \
			drivers/screen/screen.c

KERNEL_ASM_OBJ	=	$(KERNEL_ASM_SRC:.s=.o)
KERNEL_C_OBJ	=	$(KERNEL_C_SRC:.c=.o)

UNAME		:=	$(shell uname)

FLAGS		=	-Iinclude

ifeq ($(UNAME), Darwin)
	CC	=	i386-elf-gcc -g
	LD	=	i386-elf-ld
	NASM	=	nasm
endif
ifeq ($(UNAME), Linux)
	CC	=	gcc
	LD	=	ld
	NASM	=	nasm
endif

all:	$(KERNEL_IMG)

$(KERNEL_IMG):	$(BOOTLOADER) $(KERNEL)
	cat $(BOOTLOADER) $(KERNEL) > $(KERNEL_IMG)

run:	all qemue

$(BOOTLOADER):
	nasm -f bin $(BOOTLOADER_SRC) -o $(BOOTLOADER)

$(KERNEL):	$(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ)
	$(LD) -o $(KERNEL) -Ttext 0x1000 $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) --oformat binary

.c.o:
	$(CC) $(FLAGS) -ffreestanding -c $< -o $@
.s.o:
	$(NASM) $< -f elf -o $@
clean:
	rm -f	$(KERNEL_IMG)
	rm -f	$(KERNEL)
	rm -f	$(KERNEL_SYM)
	rm -f	$(BOOTLOADER)
	rm -f	$(KERNEL_C_OBJ)
	rm -f	$(KERNEL_ASM_OBJ)
qemue:
	qemu-system-i386 -fda $(KERNEL_IMG)

kernel_symbole: all
	$(LD) -o $(KERNEL_SYM) -Ttext 0x1000 $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ)

qemue-debug:	kernel_symbole
	qemu-system-i386 -s -S -fda $(KERNEL_IMG) &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

re:	clean all

.PHONY:	all clean run qemue qemue-debug kernel_symbole

