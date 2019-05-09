##
##	Makefile
##

KERNEL		=	netero
KERNEL_IMG	=	vmnetero.img
KERNEL_SYM	=	kernel.elf

ISO_FILE	=	./bin/iso/$(KERNEL_IMG)

ARCH	=	x86

all:	$(ISO_FILE)

$(ISO_FILE):
	mkdir -p bin/iso
	$(MAKE) -C ./arch/$(ARCH) all
	cp ./arch/$(ARCH)/$(KERNEL_IMG) ./bin/iso
	cp ./arch/$(ARCH)/$(KERNEL) ./bin
	$(MAKE) -C ./arch/$(ARCH) kernel_symbole
	cp ./arch/$(ARCH)/$(KERNEL_SYM) ./bin

clean:
	$(MAKE) -C ./arch/$(ARCH) clean
	rm -rf bin

run:
	qemu-system-i386 -fda ./bin/iso/$(KERNEL_IMG)

debug:
	qemu-system-i386 -s -S -fda ./bin/iso/$(KERNEL_IMG) &
	gdb -ex "target remote localhost:1234" -ex "symbol-file ./bin/$(KERNEL_SYM)"

re:	clean all

.PHONY:	all clean re qemue qemue-debug

