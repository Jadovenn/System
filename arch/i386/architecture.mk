##
## architecture.mk - Makefile config for i386
## System sources under license MIT
##

ARCH_LDFLAGS		=	-Wl,-T$(ARCH_DIR)/i386.ld

ARCH_INCLUDE_DIRS	=	$(ARCH_DIR)/ \
						$(ARCH_DIR)/include

ARCH_ASM_SRCS	=		$(ARCH_DIR)/multiboot/multiboot.s \
						$(ARCH_DIR)/cpu/asm/interrupt.s \
						$(ARCH_DIR)/cpu/asm/dt_flush.s

ARCH_C_SRCS	=	$(ARCH_DIR)/i386_entry.c \
				$(ARCH_DIR)/cpu/gdt.c \
				$(ARCH_DIR)/cpu/idt.c \
				$(ARCH_DIR)/cpu/isr.c \
				$(ARCH_DIR)/cpu/cr.c \
				$(ARCH_DIR)/cpu/dump_regs.c \
				$(ARCH_DIR)/cpu/pic/pic.c \
				$(ARCH_DIR)/cpu/pic/ports.c \
				$(ARCH_DIR)/memory/memory_layout.c \
				$(ARCH_DIR)/memory/physical/init.c \
				$(ARCH_DIR)/memory/physical/alloc.c \
				$(ARCH_DIR)/memory/virtual/init.c \
				$(ARCH_DIR)/memory/virtual/heap.c \
				$(ARCH_DIR)/memory/virtual/map.c \
				$(ARCH_DIR)/memory/virtual/alloc_pages.c \
				$(ARCH_DIR)/memory/virtual/pagefault_handler.c \
				$(ARCH_DIR)/drivers/monitor/init.c \
				$(ARCH_DIR)/drivers/monitor/monitor.c \
				$(ARCH_DIR)/drivers/timer_example/timer.c

