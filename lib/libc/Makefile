##
## Makefile - internal C library
## System sources under license MIT
##

include ../../build.config.mk
include ../../tools/config/toolchain.config.mk
include ../../tools/config/color.config.mk

NAME	=	libc.a

HEADERS	=	-Iinclude/

OBJDIR  =   ../../out/dependency/libc

SOURCES =	stdlib/abort.c \
			math/div.c \
			string/memccpy.c \
			string/memory.c \
			string/string.c \
			string/strlen.c \
			string/strstr.c

OBJS	=	$(SOURCES:%.c=$(OBJDIR)/%.o)

CFLAGS	:=	$(CFLAGS) \
			$(HEADERS) \
			-Wall \
			-Werror \
			-Wextra \
			-nostdinc \
			-nostdlib \
			-nostartfiles \
			-static-libgcc \
			-fno-pic \
			-fno-builtin \
			-ffreestanding \
			-g

all:		$(NAME)

$(NAME):	$(OBJS)
	@$(AR) rcs $(NAME) $(OBJS) && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_YELLOW) "LINK" $(COLOR_TEAL) $(NAME) $(COLOR_DEFAULT) || \
		$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_TEAL) $(NAME) $(COLOR_DEFAULT)

clean:
	@rm -f	$(NAME)
	@rm -f	$(OBJS)
	@rm -rf  $(OBJDIR)

re:	clean all

$(OBJS): $(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@  && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_TEAL) $< $(COLOR_DEFAULT) || \
		$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_TEAL) $< $(COLOR_DEFAULT)

.PHONY:	all clean re .c.o
