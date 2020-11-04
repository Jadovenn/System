##
## architecture.mk - Makefile config for i386
## System sources under license MIT
##

ARCH_LDFLAGS		=	-Wl,-T$(ARCH_DIR)/i386.ld

ARCH_INCLUDE_DIRS	=	$(ARCH_DIR)/ \
						$(ARCH_DIR)/include

ARCH_ASM_SRCS	=		$(ARCH_DIR)/init_code/multiboot.s \
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
				$(ARCH_DIR)/memory/memory_init.c \
				$(ARCH_DIR)/memory/boot/boot_paging.c \
				$(ARCH_DIR)/memory/memory_layout.c \
				$(ARCH_DIR)/memory/physical/physical_memory_init.c \
				$(ARCH_DIR)/memory/physical/physical_memory.c \
				$(ARCH_DIR)/memory/virtual/virtual_memory_init.c \
				$(ARCH_DIR)/memory/virtual/heap.c \
				$(ARCH_DIR)/memory/virtual/paging.c \
				$(ARCH_DIR)/memory/virtual/page_fault_handler.c \
				$(ARCH_DIR)/drivers/monitor/init.c \
				$(ARCH_DIR)/drivers/monitor/monitor.c \
				$(ARCH_DIR)/drivers/timer_example/timer.c

