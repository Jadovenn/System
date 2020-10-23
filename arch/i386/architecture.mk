##
## architecture.mk - Makefile config for i386
## System sources under license MIT
##

ARCH_LDFLAGS		=	-Wl,-T$(ARCH_DIR)/i386.ld

ARCH_INCLUDE_DIRS	=	$(ARCH_DIR)/include

ARCH_ASM_SRCS	=		$(ARCH_DIR)/multiboot/multiboot.s \
						$(ARCH_DIR)/cpu/asm/interrupt.s \
						$(ARCH_DIR)/cpu/asm/dt_flush.s

ARCH_C_SRCS	=	$(ARCH_DIR)/i386_entry.c \
				$(ARCH_DIR)/memlayout.c \
				$(ARCH_DIR)/timer.c \
				$(ARCH_DIR)/virtual_memory/init.c \
				$(ARCH_DIR)/virtual_memory/map.c \
				$(ARCH_DIR)/virtual_memory/alloc_pages.c \
				$(ARCH_DIR)/virtual_memory/pagefault_handler.c \
				$(ARCH_DIR)/physical_memory/init.c \
				$(ARCH_DIR)/physical_memory/physical_alloc.c \
				$(ARCH_DIR)/cpu/init/init_gdt.c \
				$(ARCH_DIR)/cpu/init/init_idt.c \
				$(ARCH_DIR)/cpu/isr.c \
				$(ARCH_DIR)/cpu/cr.c \
				$(ARCH_DIR)/cpu/dump_regs.c \
				$(ARCH_DIR)/pic/pic.c \
				$(ARCH_DIR)/pic/ports.c \
				$(ARCH_DIR)/drivers/monitor/init.c \
				$(ARCH_DIR)/drivers/monitor/monitor.c

