##
## Makefile - top level makefile
## System sources under license MIT
##

all: $(KERNEL)


##################################################
##               CONFIGURATION                  ##
##################################################

include build.config.mk
include tools/config/toolchain.config.mk
include tools/config/color.config.mk

##################################################
##                 CONSTANTS                    ##
##################################################

NAME		=	System_v$(VERSION)-$(RELEASE_NAME)
KERNEL		=	$(NAME)
ISO			=	$(NAME).iso
OBJDIR		=	out

##################################################
##                  SOURCES                     ##
##################################################

# dependency
include lib/dependencies.config.mk
include $(INCLUDE_DEPENDENCIES)
HEADERS		+= $(DEPENDENCIES_HEADERS)

# kernel common
include kernel/kernel.mk

SRCS		=	$(KERNEL_C_SRCS)
HEADERS		+=	$(addprefix -I, $(KERNEL_INCLUDE_DIRS))

# arch dependency
ARCH_DIR	=	arch/$(ARCH)
include	$(ARCH_DIR)/architecture.mk

ASM			+=	$(ARCH_ASM_SRCS)
SRCS		+=	$(ARCH_C_SRCS)
HEADERS		+=	$(addprefix -I, $(ARCH_INCLUDE_DIRS))
LDFLAGS		+=	$(ARCH_LDFLAGS)

##################################################
##                   MODES                      ##
##################################################

ifeq ($(MODE), release) ## RELEASE

SRCS		+=	$(KERNEL_ENTRY_POINT)
CFLAGS		+=	-O3

endif ## END RELEASE
ifeq ($(MODE), debug) ## DEBUG

SRCS		+=	$(KERNEL_ENTRY_POINT)
CFLAGS		+=	-g

endif ## END DEBUG
ifeq ($(MODE), test) ## TEST

include  test/make.config

SRCS		+=	$(TEST_SRCS)
HEADERS		+=	$(addprefix -I, $(TEST_HEADERS))
CFLAGS		+=	-g

endif ## END TEST

##################################################
##                  OBJECTS                     ##
##################################################

ASSEMBLY_OBJECT	+=	$(ASM:%.s=$(OBJDIR)/%.o)
C_OBJECT		=	$(SRCS:%.c=$(OBJDIR)/%.o)

OBJECTS	= $(ASSEMBLY_OBJECT) $(C_OBJECT)

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
				$(DEPENDENCIES_LD_FLAGS)

##################################################
##                  RULES                       ##
##################################################

all:					print_logo $(KERNEL)

prompt:
	@$(ECHO) $(COLOR_YELLOW) "*** GENERATING" $(COLOR_TEAL) $(KERNEL)

$(KERNEL):				build-dependencies prompt $(OBJECTS)
	@$(LD) -o $(KERNEL) $(OBJECTS) $(LDFLAGS) && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_YELLOW) "LINK" $(COLOR_TEAL) $(KERNEL) $(COLOR_DEFAULT) || \
		$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_YELLOW) "LINK" $(COLOR_TEAL) $(KERNEL)

$(C_OBJECT):			$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@ && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_TEAL) $< $(COLOR_DEFAULT) || \
		$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_TEAL) $<

$(ASSEMBLY_OBJECT):		$(OBJDIR)/%.o: %.s
	@mkdir -p $(@D)
	@$(NASM) $< -f elf -o $@ && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_TEAL) $< $(COLOR_DEFAULT) || \
	 	$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_TEAL) $<

dep_config_all:
	DEP_RULE	=	all

dep_config_clean:
	DEP_RULE	=	clean

clean:					clean-dependencies
	@$(ECHO) $(COLOR_YELLOW) "x CLEANING" $(COLOR_TEAL) $(KERNEL)
	@rm -f	$(KERNEL)
	@rm -f	$(ISO)
	@rm -f	$(OBJECTS)
	@rm -f	$(TEST_SRCS:.c=.o)
	@rm -rf	out
	@rm -f  peda-*
	@rm -f	*.sym
	@rm -f	*.log

re:		clean all

$(ISO):	$(KERNEL)
	@tools/build-iso.sh $(NAME) $(RELEASE_NAME) $(VERSION) && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_YELLOW) "GENERATE" $(COLOR_TEAL) $(ISO) $(COLOR_DEFAULT) || \
		$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_TEAL) $(ISO)

symbols-files:	$(KERNEL)
	@i386-elf-objcopy --only-keep-debug $(KERNEL) $(NAME).sym && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_YELLOW) "GENERATE" $(COLOR_TEAL) $(NAME).sym $(COLOR_DEFAULT) || \
		$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_TEAL) $(NAME).sym
	@i386-elf-nm $(NAME).sym | grep "T" | awk '{ print $$1" "$$3 }' > $(NAME).txt.sym && \
		$(ECHO) $(COLOR_GREEN) "[OK]" $(COLOR_YELLOW) "GENERATE" $(COLOR_TEAL) $(NAME).text.sym $(COLOR_DEFAULT) || \
		$(ECHO) $(COLOR_RED) "[FAILED]" $(COLOR_TEAL) $(NAME).text.sym

iso:		$(ISO)

run:			$(ISO)
	@$(ECHO) $(COLOR_YELLOW) "*** QEMU RUN" $(COLOR_DEFAULT) "$(KERNEL)"
	$(QEMU) -m $(PHYSICAL_MEM) -fda $(ISO)

run-with-gdb:	prompt symbols-files $(SYSTEM_ISO)
	@$(ECHO) $(COLOR_YELLOW) "*** QEMU RUN" $(COLOR_DEFAULT) "$(KERNEL) with gdb"
	$(QEMU) -m $(PHYSICAL_MEM) -s -S -fda $(ISO) &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL)"

run-with-gdb-server: prompt symbols-files $(SYSTEM_ISO)
	@$(ECHO) $(COLOR_YELLOW) "*** QEMU RUN" $(COLOR_DEFAULT) "$(KERNEL) with gdb server only"
	$(QEMU) -m $(PHYSICAL_MEM) -s -S -fda $(ISO)

help:
	@echo "Usage: make <rule> [OPTION]"
	@echo ""
	@echo "all:                  Build all targets."
	@echo "symbols-files:        Generate symbols files."
	@echo "iso:                  Generate a bootable iso file."
	@echo "run:                  Run kernel in qemu."
	@echo "run-with-gdb:         Run kernel in qemu with gdb."
	@echo "run-with-gdb-server:  Run kernel in qemu with gdb server only."
	@echo ""
	@echo "See build.conf for advanced option."

.PHONY:	all iso .c.o .s.o clean re run debug help

##################################################
##               Fancy stuff                    ##
##################################################

print_logo:
	@$(SET_PURP)
	@$(ECHO) "      ___                       ___                         ___           ___ "
	@$(ECHO) "     /\__\                     /\__\                       /\__\         /\  \ "
	@$(ECHO) "    /:/ _/_         ___       /:/ _/_         ___         /:/ _/_       |::\  \ "
	@$(ECHO) "   /:/ /\  \       /|  |     /:/ /\  \       /\__\       /:/ /\__\      |:|:\  \ "
	@$(ECHO) "  /:/ /::\  \     |:|  |    /:/ /::\  \     /:/  /      /:/ /:/ _/_   __|:|\:\  \ "
	@$(ECHO) " /:/_/:/\:\__\    |:|  |   /:/_/:/\:\__\   /:/__/      /:/_/:/ /\__\ /::::|_\:\__\ "
	@$(ECHO) " \:\/:/ /:/  /  __|:|__|   \:\/:/ /:/  /  /::\  \      \:\/:/ /:/  / \:\~~\  \/__/ "
	@$(ECHO) "  \::/ /:/  /  /::::\  \    \::/ /:/  /  /:/\:\  \      \::/_/:/  /   \:\  \ "
	@$(ECHO) "   \/_/:/  /   ~~~~\:\  \    \/_/:/  /   \/__\:\  \      \:\/:/  /     \:\  \ "
	@$(ECHO) "     /:/  /         \:\__\     /:/  /         \:\__\      \::/  /       \:\__\ "
	@$(ECHO) "     \/__/           \/__/     \/__/           \/__/       \/__/         \/__/ "
	@$(SET_DEFAULT)
