##
## Makefile - internal C library
## System sources under license MIT
##

NAME	=	libc.a

HEADERS	=	-Iinclude/

SOURCES	=	stdlib/abort.c \
		math/div.c \
		string/memccpy.c \
		string/memory.c \
		string/string.c \
		string/strlen.c \
		string/strstr.c

OBJS	=	$(SOURCES:.c=.o)

CFLAGS	:=	$(CFLAGS) \
		$(HEADERS) \
		-Wall \
		-Wextra \
		-nostdinc \
		-nostdlib \
		-nostartfiles \
		-static-libgcc \
		-fno-pic \
		-fno-builtin \
		-ffreestanding \
		-g

UNAME		:=	$(shell uname)
ifeq ($(UNAME), Darwin)
	CC	=	i386-elf-gcc
	AR	=	i386-elf-ar
	NASM	=	nasm
endif
ifeq ($(UNAME), Linux)
	CFLAGS	+=	-fno-pie -fno-stack-protector
	CC	=	gcc -m32 
	AR	=	ar
	NASM	=	nasm
endif

all:		$(NAME)

$(NAME):	$(OBJS)
	$(AR) rcs $(NAME) $(OBJS)

clean:
	rm -f	$(NAME)
	rm -f	$(OBJS)

re:	clean all

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:	all clean re .c.o

