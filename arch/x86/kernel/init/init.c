/**
 * init
 */

// public header
#include "kernel/log.h"

// private header
#include "init.h"

void	k_init() {
	klog_init(init_monitor(), "monitor"); 
	klog_init(init_descriptor_tables(), "reset descriptor tables");
}

