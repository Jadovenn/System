/**
 * init hooks
 */

// public header
#include "kernel/log.h"

// private header
#include "init.h"

void	early_stage() {
	init_descriptor_tables();
	klog_init(init_monitor(), "monitor"); 
	asm volatile ("sti");
}

