/**
 * init
 */

// public header
#include "kernel/log.h"

// private header
#include "init.h"

void	k_init() {
	klog_init(init_screen(), "screen"); 
}

