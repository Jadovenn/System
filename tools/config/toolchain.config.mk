
UNAME		:=	$(shell uname)

ifeq ($(ARCH), i386) ## i386

ifeq ($(UNAME), Darwin) ## DARWIN
	CC	=	i386-elf-gcc
	LD	=	$(CC)
	AR	=	i386-elf-ar
	NASM	=	nasm
endif ## END DARWIN
ifeq ($(UNAME), Linux) ## LINUX BASED
	CFLAGS	+=	-fno-pie -fno-stack-protector
	CC	=	gcc -m32
	LD	=	$(CC)
	NASM	=	nasm
endif # END LINUX BASED

endif ## END x86
