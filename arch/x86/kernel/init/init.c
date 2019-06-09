/**
 * init.c - init hooks to execute by the early_stage
 * System sources under license MIT
 */

#include <kernel/log.h>

#include "init.h"

void	early_stage() {
	init_descriptor_tables();
	klog_init(init_monitor(), "monitor"); 
	asm volatile ("sti");
}

